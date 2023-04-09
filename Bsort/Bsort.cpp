#include <iostream>

#include "Bsort.hpp"

#define BSORT_INIT         "bsort_init"
#define BSORT_STAGE_0      "bsort_stage_0"
#define BSORT_STAGE_N      "bsort_stage_n"
#define BSORT_MERGE        "bsort_merge"
#define BSORT_MERGE_LAST   "bsort_merge_last"

opencl::Bsort_app::Bsort_app() : 
                                platform_{select_platform()}, 
                                context_{get_gpu_context(platform_())},
                                device_{select_device()},
                                queue_{context_, device_, cl::QueueProperties::Profiling | 
                                                        cl::QueueProperties::OutOfOrder},
                                kernel_source_{create_kernel_source(BITONIC_PATH)},
                                program_{build_program()} {}

cl::Platform opencl::Bsort_app::select_platform() {
    cl::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::vector<cl::Device> devices;
    for (auto& p : platforms) {
        p.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        if (devices.size() > 0) {
            return p;
        }
    }
    throw std::runtime_error("No platform selected");
}

cl::Context opencl::Bsort_app::get_gpu_context(cl_platform_id PId) {
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(PId),
        0 // signals end of property list
    };

    return cl::Context(CL_DEVICE_TYPE_GPU, properties);
}

std::string opencl::Bsort_app::create_kernel_source(const char *filepath) {
    std::string source;
    std::ifstream kernel_file;
    kernel_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    kernel_file.open(filepath);
    std::stringstream kernel_stream;
    kernel_stream << kernel_file.rdbuf();
    kernel_file.close();
    source = kernel_stream.str();
    return source;
}

cl::Device opencl::Bsort_app::select_device() {
    std::vector<cl::Device> devices = context_.getInfo<CL_CONTEXT_DEVICES>();
    return devices.front();
}

cl::Program opencl::Bsort_app::build_program() { 
    return cl::Program(context_, kernel_source_, true /* build immediately */); 
}

cl::vector<cl::Kernel> opencl::Bsort_app::get_program_kernels() {
    cl::vector<cl::Kernel> kernels;
    program_.createKernels(&kernels);
    return kernels;
}

void opencl::Bsort_app::get_info() {
    std::cout << "Platform: " << platform_.getInfo<CL_PLATFORM_NAME>() << std::endl;
    auto devices = context_.getInfo<CL_CONTEXT_DEVICES>();
    std::cout << "Devices:\n";
    for (auto& d : devices) {
        std::cout << d.getInfo<CL_DEVICE_NAME>() << std::endl << std::endl;
    }
}

size_t opencl::Bsort_app::get_work_group_size(cl::Kernel& kernel) {
    size_t local_size = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device_);
    return static_cast<size_t>(std::pow(2, std::trunc(std::log2(local_size))));
}

size_t opencl::Bsort_app::bitonic_sort(std::vector<float>& data_vec) {

    if (data_vec.size() == 0) return 0;

    cl::vector<cl::Kernel> kernels = get_program_kernels();
    
    cl::Kernel& kernel_init = kernels[0];
    cl::Kernel& kernel_stage_0 = kernels[1];
    cl::Kernel& kernel_stage_n = kernels[2];
    cl::Kernel& kernel_merge = kernels[3];
    cl::Kernel& kernel_merge_last = kernels[4];

    size_t l_size = get_work_group_size(kernel_init);

    size_t initial_size = data_vec.size();
    size_t aligned_size = align_to_power_of_2(data_vec.size());
    data_vec.resize(std::max(aligned_size, l_size), CL_FLT_MAX);

    cl::Buffer data_buffer(context_, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, data_vec.size() * sizeof(float), data_vec.data());
    cl::LocalSpaceArg local_memory = cl::Local(8 * sizeof(float) * l_size);

    size_t g_size = data_vec.size() / 8;
    if (g_size < l_size) { l_size = g_size; }

    cl::NDRange local_size(l_size);
    cl::NDRange global_size(g_size);

    cl::Event profiling_evt; 
    auto start = std::chrono::steady_clock::now();

    for (auto& kernel : kernels) {
        kernel.setArg(0, data_buffer);
        kernel.setArg(1, local_memory);
    }
    execute(kernel_init, global_size, local_size);

    size_t num_stages = g_size / l_size;
    for(size_t high_stage = 2; high_stage < num_stages; high_stage <<= 1) {
        kernel_stage_0.setArg(2, high_stage);
        kernel_stage_n.setArg(3, high_stage);

        for (size_t stage = high_stage; stage > 1; stage >>= 1) {
            kernel_stage_n.setArg(2, stage);
            execute(kernel_stage_n, global_size, local_size);
        }

        execute(kernel_stage_0, global_size, local_size);   
    }
    

    kernel_merge.setArg(3, direction_);
    kernel_merge_last.setArg(2, direction_);

    for (size_t stage = num_stages; stage > 1; stage >>= 1) {
        kernel_merge.setArg(2, stage);
        execute(kernel_merge, global_size, local_size);
    }

    execute(kernel_merge_last, global_size, local_size);
    
    auto end = std::chrono::steady_clock::now();    
    
    cl::copy(queue_, data_buffer, data_vec.begin(), data_vec.end());
    data_vec.resize(initial_size);

    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

size_t opencl::Bsort_app::align_to_power_of_2 (size_t size) {
    return static_cast<size_t> (std::pow(2, std::ceil(std::log2(size))));
}

void opencl::Bsort_app::execute (cl::Kernel& kernel, cl::NDRange& global_size, cl::NDRange& local_size) {
    cl::Event evt;
    queue_.enqueueNDRangeKernel(kernel, 0, global_size, local_size, nullptr, &evt);
    evt.wait();
} 

