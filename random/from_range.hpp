#pragma once

#include <random>


namespace random {

template <typename T> 
T from_range(T min, T max, std::mt19937& gen) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(gen);
}

int from_range(int min, int max, std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

}
