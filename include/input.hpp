#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <iostream>
#include <stdexcept>
#include <limits>

namespace input 
{

void handle_input_error() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if ((std::cin >> std::ws).eof()) {
        throw std::runtime_error("Input: EOF reached");
    }
}

template<typename T> 
T get_val() {
    T value;
    std::cin >> value;

    while (!(std::cin && (std::isspace(std::cin.peek()) || std::cin.eof()))) {
        handle_input_error();
        std::cin >> value;
    }

    return value;
}

std::vector<float> get_data(size_t size) {
    std::vector<float> vec(size);
    std::generate(vec.begin(), vec.end(), []{ return get_val<float>(); });
    return vec;
}

} // input
