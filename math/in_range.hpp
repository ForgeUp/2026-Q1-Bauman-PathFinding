#pragma once

#include <cmath>


namespace math {

bool in_range(double val, double l, double r) {
    return l <= val && val < r;
}

}
