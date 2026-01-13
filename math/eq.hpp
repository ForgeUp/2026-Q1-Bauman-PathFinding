#pragma once

#include <cmath>


namespace math {

bool eq(double r, double l, double eps = 1e-12) {
    return std::fabs(r - l) < eps;
}

}
