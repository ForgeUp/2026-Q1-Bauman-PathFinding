#pragma once

#include <cmath>


namespace math {

double trunc(double value, double digits = 6) {
    double factor = std::pow(10.0, digits);
    return std::floor(value * factor) / factor;
}

}
