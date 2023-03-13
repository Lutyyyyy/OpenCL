#include <vector>
#include <algorithm>

#include "input.hpp"
#include "OpenCL_app.hpp"
#include "gtest/gtest.h"
#include "test_generator.hpp"

#define BITONIC_PATH "../../kernels/bitonic_sort.cl"
#define DEFAULT_SIZE 65536

namespace testing
{

TEST(correctness, Test1)
{
    std::vector<float> vector = generate_random_sequence(DEFAULT_SIZE);
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);   
}

TEST(correctness, Test2)
{
    std::vector<float> vector = generate_ascending_sequence(DEFAULT_SIZE);
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);  
}

TEST(correctness, Test3)
{
    std::vector<float> vector = generate_descending_sequence(DEFAULT_SIZE);
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);  
}

TEST(correctness, Test4)
{
    std::vector<float> vector = generate_bitonic_sequence(DEFAULT_SIZE);
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);  
}

TEST(correctness, Test5)
{
    std::vector<float> vector(DEFAULT_SIZE, 1.0);
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);  
}

TEST(correctness, Test6)
{
    std::vector<float> vector = generate_random_sequence(50);
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);  
}

TEST(correctness, Test7)
{
    std::vector<float> vector = {1};
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);  
}

TEST(correctness, Test8)
{
    std::vector<float> vector;
    opencl::OpenCL_app app{BITONIC_PATH};
    app.bitonic_sort(vector);
    EXPECT_EQ (std::is_sorted(vector.begin(), vector.end()), true);  
}

} //testing


int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}