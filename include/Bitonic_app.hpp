#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "CL/opencl.hpp"
#define __CL_ENABLE_EXCEPTIONS

#define LOG_PATH       "../"

#define DIRECTION 0 /*Ascending: 0; descending: -1*/

#ifndef PROFILING_ENABLED
#define PROFILING_ENABLED 1
#endif

#ifndef CHECK_ENABLED 
#define CHECK_ENABLED 0
#endif


namespace opencl
{

class Bitonic_app {

private:
    cl::Platform platform_;
    cl::Context context_;
    cl::Device device_;
    cl::CommandQueue queue_;

    std::string kernel_source_;
   
public:
    
    Bitonic_app(const char* filepath) : platform_{select_platform()}, 
                                        context_{get_gpu_context(platform_())}, 
                                        device_{select_device()},
                                        queue_{context_, device_, cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder},
                                        kernel_source_{create_kernel_source(filepath)} {}
    
    static cl::Platform select_platform();
    static cl::Context get_gpu_context(cl_platform_id);
    static std::string create_kernel_source(const char * filepath);
    cl::Device select_device();
    
    size_t get_work_group_size (cl::Kernel& kernel);
    cl_ulong event_duration (cl::Event& e);  

    void get_info(); 

//bitonic_sort functions 
    void bitonic_sort(std::vector<float>& data_vec);
    size_t align_to_power_of_2 (size_t size); 
};

} //opencl 
