#include <charconv>
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>


#define COMPARE_CPU 1


#include "input.hpp"
#include "Bitonic_app.hpp"


int main () try {

    opencl::Bitonic_app app{"../../../kernels/bitonic_sort.cl"};
    app.get_info();
    std::vector<float> vec = input::get_data<float>();
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

} catch (const std::exception &e) {
    std::cerr << e.what() << std::endl; 
    return EXIT_FAILURE;
}
