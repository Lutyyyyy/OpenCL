#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <fstream>


#define COMPARE_CPU 1


#include "OpenCL_app.hpp"
#include "input.hpp"

int main () try {

    opencl::OpenCL_app app{"../../../kernels/bitonic_sort.cl"};
    //app.get_info();
    size_t size = input::get_val<size_t>();
    std::vector<float> vec = input::get_data(size);
    std::vector<float> vec_copy(vec);

    std::chrono::steady_clock::time_point bitonic_begin = std::chrono::steady_clock::now();
    app.bitonic_sort(vec_copy);
    std::chrono::steady_clock::time_point bitonic_end = std::chrono::steady_clock::now();
    std::cout << "GPU wall time: " << std::chrono::duration_cast<std::chrono::milliseconds>(bitonic_end - bitonic_begin).count() << " [ms]" << std::endl;
    
    if (COMPARE_CPU) { 
        std::vector<float> vec_copy_cpu (vec); 
        std::chrono::steady_clock::time_point cpu_begin = std::chrono::steady_clock::now();
        std::sort(vec_copy_cpu.begin(), vec_copy_cpu.end());
        std::chrono::steady_clock::time_point cpu_end = std::chrono::steady_clock::now();
        std::cout << "\nCPU time: " << std::chrono::duration_cast<std::chrono::microseconds>(cpu_end - cpu_begin).count() / 1000.0 << "[ms]" << std::endl;
    }
    
    return EXIT_SUCCESS;

} catch (cl::Error &e) {
        std::cout << "clError: " << e.err() << " " << e.what() << std::endl;
} catch (std::exception &e) {
    std::cerr << e.what() << std::endl; 
} 
