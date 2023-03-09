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

template<typename T>
std::vector<T> get_data() {
    size_t size = get_val<size_t>();
    std::vector<T> data_vector(size);
    for (size_t i = 0; i < size; ++i) {
        data_vector[i] = get_val<T>();
    }
    
    return data_vector;
}

template<typename T>
std::vector<T> get_data_from_file(const char* filepath) {
    std::ifstream stream(filepath);
    size_t size = 0;
    stream >> size;
    std::vector<T> data_vector(size);
    for (size_t i = 0; i < size; ++i) {
        stream >> data_vector[i];
    }
    for (size_t i = 0; i < size; ++i) {
        std::cout << data_vector[i] << std::endl;
    }
    stream.close();
    return data_vector;
}

} // input
