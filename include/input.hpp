#pragma once

#include <vector>
#include <random>
#include <fstream>
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

    if (std::cin.eof()) {
        throw std::runtime_error("Input: EOF reached");
    }

    while((!std::cin) || (!std::isspace(std::cin.peek()))) {
        handle_input_error();
        std::cin >> value;
    }

    return value;
}

void generate_data(const char* filepath, unsigned long long size) {
    
    std::mt19937 gen(rand());
    std::uniform_real_distribution<float> dis(-100000.0, 100000.0);
    
    std::fstream input(filepath);
    for (int i = 0; i < size; ++i) {
        input << dis(gen) << ' ';
    }
    input << '\n';

    input.close();
}

} // input
