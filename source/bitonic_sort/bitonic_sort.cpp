#include <iostream>

#include "OpenCL_app.hpp"

#define BSORT_INIT         "bsort_init"
#define BSORT_STAGE_0      "bsort_stage_0"
#define BSORT_STAGE_N      "bsort_stage_n"
#define BSORT_MERGE        "bsort_merge"
#define BSORT_MERGE_LAST   "bsort_merge_last"


cl::Platform opencl::OpenCL_app::select_platform() {
    cl::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    for (auto p : platforms) {
        cl_uint numdevices = 0;
        ::clGetDeviceIDs(p(), CL_DEVICE_TYPE_GPU, 0, NULL, &numdevices); //p() for plain id
        if (numdevices > 0)
        return cl::Platform(p);
    }
    throw std::runtime_error("No platform selected");
}

cl::Context opencl::OpenCL_app::get_gpu_context(cl_platform_id PId) {
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(PId),
        0 // signals end of property list
    };

    return cl::Context(CL_DEVICE_TYPE_GPU, properties);
}

std::string opencl::OpenCL_app::create_kernel_source(const char *filepath) {
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

cl::Device opencl::OpenCL_app::select_device() {
    std::vector<cl::Device> devices = context_.getInfo<CL_CONTEXT_DEVICES>();
    return devices.front();
}

cl::Program opencl::OpenCL_app::build_program() { 
    return cl::Program(context_, kernel_source_, true /* build immediately */); 
}

cl::vector<cl::Kernel> opencl::OpenCL_app::get_program_kernels() {
    cl::vector<cl::Kernel> kernels;
    program_.createKernels(&kernels);
    return kernels;
}

void opencl::OpenCL_app::get_info() {
    std::cout << "Platform: " << platform_.getInfo<CL_PLATFORM_NAME>() << std::endl;
    auto devices = context_.getInfo<CL_CONTEXT_DEVICES>();
    std::cout << "Devices:\n";
    for (auto& d : devices) {
        std::cout << d.getInfo<CL_DEVICE_NAME>() << std::endl << std::endl;
    }
}

size_t opencl::OpenCL_app::get_work_group_size(cl::Kernel& kernel) {
    size_t local_size = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device_);
    return static_cast<size_t>(std::pow(2, std::trunc(std::log2(local_size))));
}

cl_ulong opencl::OpenCL_app::event_duration (cl::Event& e) {
    cl_ulong start = e.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    cl_ulong end = e.getProfilingInfo<CL_PROFILING_COMMAND_END>();
    return end - start;
}

size_t opencl::OpenCL_app::bitonic_sort(std::vector<float>& data_vec) {

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
    
    kernel_init.setArg(0, data_buffer);
    kernel_stage_0.setArg(0, data_buffer);
    kernel_stage_n.setArg(0, data_buffer);
    kernel_merge.setArg(0, data_buffer);
    kernel_merge_last.setArg(0, data_buffer);

    cl::LocalSpaceArg local_memory = cl::Local(8 * sizeof(float) * l_size);
    
    kernel_init.setArg(1, local_memory);
    kernel_stage_0.setArg(1, local_memory);
    kernel_stage_n.setArg(1, local_memory);
    kernel_merge.setArg(1, local_memory);
    kernel_merge_last.setArg(1, local_memory);

    size_t g_size = data_vec.size() / 8;
    if (g_size < l_size) { l_size = g_size; }

    cl::NDRange local_size(l_size);
    cl::NDRange global_size(g_size);

    cl::Event profiling_evt; 
    cl_ulong time_used = 0;

    queue_.enqueueNDRangeKernel(kernel_init, 0, global_size, local_size, nullptr, &profiling_evt);
    queue_.finish();
    time_used += event_duration(profiling_evt);

    size_t num_stages = g_size / l_size;

    for(size_t high_stage = 2; high_stage < num_stages; high_stage <<= 1) {
        kernel_stage_0.setArg(2, high_stage);
        kernel_stage_n.setArg(3, high_stage);

        for (size_t stage = high_stage; stage > 1; stage >>= 1) {
            kernel_stage_n.setArg(2, stage);
            queue_.enqueueNDRangeKernel(kernel_stage_n, 0, global_size, local_size, nullptr, &profiling_evt);
            queue_.finish();
            time_used += event_duration(profiling_evt);
        }

        queue_.enqueueNDRangeKernel(kernel_stage_0, 0, global_size, local_size, nullptr, &profiling_evt);
        queue_.finish();
        time_used += event_duration(profiling_evt);    
    }

    int direction = DIRECTION;

    kernel_merge.setArg(3, direction);
    kernel_merge_last.setArg(2, direction);

    for (size_t stage = num_stages; stage > 1; stage >>= 1) {
        kernel_merge.setArg(2, stage);
        
        queue_.enqueueNDRangeKernel(kernel_merge, 0, global_size, local_size, nullptr, &profiling_evt);
        queue_.finish();
        time_used += event_duration(profiling_evt); 
    }

    queue_.enqueueNDRangeKernel(kernel_merge_last, 0, global_size, local_size, nullptr, &profiling_evt);
    queue_.finish();
    time_used += event_duration(profiling_evt); 

    cl::copy(queue_, data_buffer, data_vec.begin(), data_vec.end());
    data_vec.resize(initial_size);

    return time_used;
}

size_t opencl::OpenCL_app::align_to_power_of_2 (size_t size) {
    return static_cast<size_t> (std::pow(2, std::ceil(std::log2(size))));
}

