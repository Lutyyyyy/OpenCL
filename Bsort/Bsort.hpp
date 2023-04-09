#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 210
#define CL_HPP_ENABLE_EXCEPTIONS

#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "CL/opencl.hpp"

#ifndef BITONIC_PATH
    #define BITONIC_PATH "../Bsort/Bsort.cl"
#endif

namespace opencl
{

enum class Direction { ASCENDING = 0, DESCENDING = 1 };

class Bsort_app final {

private:
    cl::Platform platform_;
    cl::Context context_;
    cl::Device device_;
    cl::CommandQueue queue_;
    std::string kernel_source_;
    cl::Program program_;
   
public:
    static constexpr int direction_ = static_cast<int>(Direction::ASCENDING);

    Bsort_app(); 
    
    Bsort_app(Bsort_app&&) = delete;
    Bsort_app &operator=(Bsort_app&&) = delete;
    Bsort_app(const Bsort_app&) = delete;
    Bsort_app &operator=(const Bsort_app&) = delete;
    
    static cl::Platform select_platform();
    static cl::Context get_gpu_context(cl_platform_id);
    static std::string create_kernel_source(const char * filepath);
    
    cl::Device select_device();
    cl::Program build_program();
    cl::vector<cl::Kernel> get_program_kernels();

    size_t get_work_group_size (cl::Kernel& kernel);
    void execute(cl::Kernel& kernel, cl::NDRange& global_size, cl::NDRange& local_size); 

    void get_info(); 

//bitonic_sort functions 
    size_t bitonic_sort(std::vector<float>& data_vec);
    size_t align_to_power_of_2 (size_t size); 
};

} //opencl 
