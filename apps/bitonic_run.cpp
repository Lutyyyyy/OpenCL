#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <fstream>

#define DEBUG_MODE 

#ifdef DEBUG_MODE
    std::ofstream log_file("log.txt", std::ios::trunc);
    
    #define LOG(message)                                       \
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


#define CHECK_ENABLED     1
#define PROFILING_ENABLED 1
#define COMPARE_CPU       1


#include "OpenCL_app.hpp"
#include "input.hpp"

int main () try {

    opencl::OpenCL_app app{};
    LOG("app successfully created")
    app.get_info();

    size_t size = input::get_val<size_t>();
    std::vector<float> vec = input::get_data(size);
    
    LOG("data vector created")
    ASSERT(vec.size() == size, "vector size equals input size")
    if (vec.size() == size) {
        LOG_VECTOR_INFO(vec)
    }

    std::vector<float> vec_copy(vec);
    LOG("Bitonic sort started")
    std::chrono::steady_clock::time_point bitonic_begin = std::chrono::steady_clock::now();
    size_t gpu_time = app.bitonic_sort(vec_copy);
    std::chrono::steady_clock::time_point bitonic_end = std::chrono::steady_clock::now();
    LOG("Bitonic sort ended\nSorted vector info:")
    LOG_VECTOR_INFO(vec_copy)

    for (size_t i = 0; i < vec_copy.size(); ++i) {
        std::cout << vec_copy[i] << ' ';
    }
    std::cout << std::endl;

    if (CHECK_ENABLED) {
        #ifdef DEBUG_MODE                                 
            #define error_file log_file
        #else
            std::ofstream error_file("check.txt", std::ios::trunc);
        #endif

        error_file << "CHECK flag enabled\nChecking started\n";

        size_t errors = 0;
        for (size_t i = 0; i < vec_copy.size() - 1; ++i) {
            if (vec_copy[i] > vec_copy[i+1]) {
                errors++;
                error_file << "sorted[" << i << "] > sorted[" << i + 1 << "] \t (" << vec_copy[i] << " > " << vec_copy[i+1] << ")\n"; 
            }
        }
        
        if (errors) { 
            error_file << "Checking ended with total " << errors << " errors\n\n";
            std::cout << "Checking ended with " << errors << " errors. See check.txt (log.txt if DEBUG MODE) for futher information.\n"; 
        }
        else { 
            error_file << "Checking ended without errors\n";
            std::cout << "GPU sorted correctly\n\n"; 
        }
        
        #ifndef DEBUG_MODE 
            error_file.close();
        #endif
    }

    if (PROFILING_ENABLED) {
        LOG("PROFILING flag enabled\nProfiling started")
        std::cout << "GPU wall time: " << std::chrono::duration_cast<std::chrono::milliseconds>(bitonic_end - bitonic_begin).count() << " [ms]" << std::endl;
        std::cout << "GPU pure time: " << gpu_time / 1e6 /*ns->ms*/ << " [ms]" << std::endl;
        LOG("Profiling ended")
    }

    if (COMPARE_CPU) { 
        LOG("COMPARE_CPU flag enabled\nCreating copy of input vector and running std::sort")
        std::vector<float> vec_copy_cpu (vec); 
        LOG("copy of input vector created")
        LOG_VECTOR_INFO(vec_copy_cpu)
        LOG("std::sort started")
        std::chrono::steady_clock::time_point cpu_begin = std::chrono::steady_clock::now();
        std::sort(vec_copy_cpu.begin(), vec_copy_cpu.end());
        std::chrono::steady_clock::time_point cpu_end = std::chrono::steady_clock::now();
        LOG("std::sort ended\nSorted vector info:")
        LOG_VECTOR_INFO(vec_copy_cpu)
        std::cout << "\nCPU time: " << std::chrono::duration_cast<std::chrono::microseconds>(cpu_end - cpu_begin).count() / 1000.0 << "[ms]" << std::endl;
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
