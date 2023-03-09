#include <fstream>
#include <random>

std::vector<float> generate_random_sequence       (size_t size);
std::vector<float> generate_ascending_sequence    (size_t size);
std::vector<float> generate_descending_sequence   (size_t size);
std::vector<float> generate_bitonic_sequence      (size_t size);


std::vector<float> generate_random_sequence(size_t size) {
    
    std::mt19937 gen(rand());
    std::uniform_real_distribution<float> dis(-1000000.0, 1000000.0);
    
    std::vector<float> vec(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = dis(gen);
    }
    
    return vec;
}

std::vector<float> generate_ascending_sequence(size_t size) {
      
    std::vector<float> vec(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = (float) (i / 1000 + 2 * i / size);
    }

    return vec;
}

std::vector<float> generate_descending_sequence(size_t size) {
      
    std::vector<float> vec(size);
    for (size_t i = size; i > 0; --i) {
        vec[i] = (float) (i / 1000 - 2 * i / size);
    }

    return vec;
}

std::vector<float> generate_bitonic_sequence(size_t size) {
      
    std::vector<float> vec(size);
    for (size_t i = 0; i < size / 2; ++i) {
        vec[i] = (float) (i / 1000);
    }

    for (size_t i = size / 2; i < size; ++i) {
        vec[i] = (float) ((size - i - 1) / 1000);
    }
    
    return vec;
}
