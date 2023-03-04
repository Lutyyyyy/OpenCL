#include "input.hpp"
#include <chrono>
#include <algorithm>

int main()
{
    unsigned long long size = input::get_val<int>();
    
    input::generate_data("input.txt", size);

    std::fstream file("input.txt");

    std::vector<float> vec(size);
    for (unsigned long long i = 0; i < size; ++i) {
        file >> vec[i];
    } 

    auto start = std::chrono::high_resolution_clock::now();
    std::sort(vec.begin(), vec.end());
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " ms" << std::endl;

    for (unsigned long long i = 0; i < size; ++i) {
        std::cout << vec[i] << ' ';
    }
    std::cout << '\n';
} 