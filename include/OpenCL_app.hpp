#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 210
#define CL_HPP_ENABLE_EXCEPTIONS

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "CL/opencl.hpp"

#define DIRECTION 0 /*Ascending: 0; descending: -1*/

namespace opencl
{

class OpenCL_app final {

private:
    cl::Platform platform_;
    cl::Context context_;
    cl::Device device_;
    cl::CommandQueue queue_;

    std::string kernel_source_;
    cl::Program program_;
   
public:
    OpenCL_app(const char* filepath) : platform_{select_platform()}, 
                                       context_{get_gpu_context(platform_())}, 
                                       device_{select_device()},
                                       queue_{context_, device_, cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder},
                                       kernel_source_{create_kernel_source(filepath)},
                                       program_{build_program()} {}
    
    OpenCL_app(OpenCL_app&&) = delete;
    OpenCL_app &operator=(OpenCL_app&&) = delete;
    OpenCL_app(const OpenCL_app&) = delete;
    OpenCL_app &operator=(const OpenCL_app&) = delete;
    
    static cl::Platform select_platform();
    static cl::Context get_gpu_context(cl_platform_id);
    static std::string create_kernel_source(const char * filepath);
    
    cl::Device select_device();
    cl::Program build_program();
    cl::vector<cl::Kernel> get_program_kernels();

    size_t get_work_group_size (cl::Kernel& kernel);
    cl_ulong event_duration (cl::Event& e);  

    void get_info(); 

//bitonic_sort functions 
    size_t bitonic_sort(std::vector<float>& data_vec);
    size_t align_to_power_of_2 (size_t size); 
};

} //opencl 
