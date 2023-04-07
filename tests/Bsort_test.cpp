#include <vector>
#include <algorithm>

#include "../Bsort/Bsort.hpp"
#include "gtest/gtest.h"
#include "test_generator.hpp"

namespace testing
{

TEST(correctness, Test1)
{
    EXPECT_EQ(test_generated_sequence(&generate_random_sequence), true);
}

TEST(correctness, Test2)
{
    EXPECT_EQ(test_generated_sequence(&generate_random_sequence, 50), true);
}

TEST(correctness, Test3)
{
    EXPECT_EQ(test_generated_sequence(&generate_ascending_sequence), true);
}

TEST(correctness, Test4)
{
    EXPECT_EQ(test_generated_sequence(&generate_descending_sequence), true);
}

TEST(correctness, Test5)
{
    EXPECT_EQ(test_generated_sequence(&generate_bitonic_sequence), true);
}

TEST(correctness, Test6)
{
    std::vector<float> vector(DEFAULT_SIZE, 1.0);
    EXPECT_EQ(test_sequence(vector), true);
}

TEST(correctness, Test7)
{
    std::vector<float> vector = {1};
    EXPECT_EQ(test_sequence(vector), true);
}

TEST(correctness, Test8)
{
    std::vector<float> vector;
    EXPECT_EQ(test_sequence(vector), true);
}

} //testing


int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}