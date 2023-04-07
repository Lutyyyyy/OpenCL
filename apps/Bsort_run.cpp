#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <execution>

#ifdef DEBUG_MODE
    std::ofstream log_file("log.txt", std::ios::trunc);
    
    #define LOG(message)                                        \
        log_file << __FILE__ << ":\nLine " << __LINE__ << ":\n"; \
        log_file << message << "\n\n";  
    
    #define LOG_VECTOR_INFO(vector)                                 \
        log_file << "vector_size = " << vector.size() << "\ndata: "; \
        for (size_t i = 0; i < vector.size(); ++ i) {                 \
            log_file << vector[i] << ' ';                              \
        }                                                               \
        log_file << "\n\n";
    #define ASSERT(statement, message)                                                                \
        if (!(statement)) {                                                                            \
            log_file << message << "\n";                                                                \
            log_file << "An error has occured in file:" << __FILE__ << "\nLine: " << __LINE__ << "\n";   \
            log_file << "While executing function: " << __PRETTY_FUNCTION__ << "\n";                      \
        }
    #define LOG_ERROR(error)                                                                       \
        log_file << "Error: " << e.what() << std::endl;                                             \
        log_file << "An error has occured in file:" << __FILE__ << "\nLine: " << __LINE__ << "\n";   \
        log_file << "While executing function: " << __PRETTY_FUNCTION__ << "\n";                      
    #define LOG_CL_ERROR(error)                                                                            \
        log_file << "clError: " << e.err() << " " << e.what() << std::endl;                                 \
        log_file << "An error has occured in file:" << __FILE__ << "\nLine: " << __LINE__ << "\n";           \
        log_file << "While executing function: " << __PRETTY_FUNCTION__ << "\n";                      
#else
    #define LOG(message)
    #define LOG_VECTOR_INFO(vector) 
    #define ASSERT(statement, message) 
    #define LOG_ERROR(error)
    #define LOG_CL_ERROR(error)
#endif

#include "../Bsort/Bsort.hpp"
#include "input.hpp"
#include "program_options.hpp"

int main (int argc, char* argv[]) try {
    Command_line_options options;
    Command_line_options::Return_status status = options.parse(argc, argv);

    if (status == Command_line_options::Return_status::OPTS_HELP) {
        return EXIT_SUCCESS;
    }
    else if (status == Command_line_options::Return_status::OPTS_SUCCESS)  {
        opencl::Bsort_app app{};
        LOG("app successfully created")
        if (options.device_info_flag()) {
            app.get_info();
        }

        size_t size = input::get_val<size_t>();
        std::vector<float> vec = input::get_data(size);
        
        LOG("data vector created")
        ASSERT(vec.size() == size, "vector size equals input size")
        LOG_VECTOR_INFO(vec)

        std::vector<float> vec_copy(vec);
        LOG("Bitonic sort started")
        size_t gpu_time = app.bitonic_sort(vec_copy);
        LOG("Bitonic sort ended\nSorted vector info:")
        LOG_VECTOR_INFO(vec_copy)

        for (size_t i = 0; i < vec_copy.size(); ++i) {
            std::cout << vec_copy[i] << ' ';
        }
        std::cout << std::endl;

        if (options.check_flag()) {          
            if ( std::is_sorted(vec_copy.begin(), vec_copy.end()) == false ) { 
                std::cout << "vector is sorted incorrectly\n";
            }
            else { 
                std::cout << "GPU sorted correctly\n"; 
            }
        }

        if (options.profiling_flag()) {
            LOG("PROFILING flag enabled\nProfiling started")
            std::cout << "GPU time: " << gpu_time / 1000000.0 << " [ms]" << std::endl;
            LOG("Profiling ended")
        }

        if (options.cpu_compare_flag()) {
            LOG("COMPARE_CPU flag enabled\nCreating copy of input vector and running std::sort")
            std::vector<float> vec_copy_cpu (vec); 
            LOG("copy of input vector created")
            LOG_VECTOR_INFO(vec_copy_cpu)
            LOG("std::sort started")
            std::chrono::steady_clock::time_point cpu_begin = std::chrono::steady_clock::now();
            std::sort(std::execution::par_unseq, vec_copy_cpu.begin(), vec_copy_cpu.end());
            std::chrono::steady_clock::time_point cpu_end = std::chrono::steady_clock::now();
            LOG("std::sort ended\nSorted vector info:")
            LOG_VECTOR_INFO(vec_copy_cpu)
            if (!options.profiling_flag()) std::cout << "GPU time: " << gpu_time / 1000000.0 << " [ms]" << std::endl;
            std::cout << "\nstd::sort(par_unseq policy) time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_end - cpu_begin).count() / 1000000.0 << "[ms]" << std::endl;
        }
    }
       
    #ifdef DEBUG_MODE 
        log_file.close();
    #endif
    
    LOG("program ended with EXIT_SUCCESS")
    return EXIT_SUCCESS;

} catch (cl::Error &e) {
    LOG_CL_ERROR(e)
    std::cout << e.err() << " " << e.what() << std::endl;
} catch (std::exception &e) {
    LOG_ERROR(e)
    std::cerr << e.what() << std::endl; 
} 
