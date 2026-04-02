#pragma once

#include <chrono>


class Timer {
private:
    std::chrono::high_resolution_clock::time_point begin;

public:
    Timer() : begin{std::chrono::high_resolution_clock::now()} {}

    template<typename Unit = std::chrono::milliseconds>
    Unit tick() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<Unit>(end - begin);
    }
};
