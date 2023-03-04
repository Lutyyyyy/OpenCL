


#include "CL/opencl.hpp"


class OclApp {
    cl::Platform platform_;
    cl::Context context_;
    cl::CommandQueue queue_;
    std::string source_;

    static cl::Platform select_platform();
    static cl::Context get_gpu_context(cl_platform_id);
    static std::string create_source_file(const char *);

public:
    OclApp();
};

