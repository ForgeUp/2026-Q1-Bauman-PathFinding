#pragma once

#include <random>


namespace random {

int from_norm(double mean, double sigma, std::mt19937& gen) {
    static thread_local std::normal_distribution<double> dist(mean, sigma);
    return dist(gen);
}

}
