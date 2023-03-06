#include <vector>
#include <algorithm>

#include "OpenCL_app.hpp"
#include "gtest/gtest.h"


namespace OpenCL
{
namespace testing
{

TEST(correctness, Test1)
{
    //sort test1.dat + compare with std::sort
    EXPECT_EQ (0, false);   
}

TEST(correctness, Test2)
{
    //sort test2.dat + compare with std::sort  
    EXPECT_EQ (0, false);
}

TEST(correctness, Test3)
{
    //sort test3.dat + compare with std::sort  
    EXPECT_EQ (0, false);   
}

TEST(correctness, Test4)
{
    //sort test4.dat + compare with std::sort  
    EXPECT_EQ (0, false);  
}

TEST(correctness, Test5)
{
    //sort test5.dat + compare with std::sort  
    EXPECT_EQ (0, false);
}

TEST(correctness, Test6)
{
    //sort test6.dat + compare with std::sort  
    EXPECT_EQ (0, false);
}

} //testing

} //OpenCL

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}