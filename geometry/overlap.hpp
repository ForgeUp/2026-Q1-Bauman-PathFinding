#pragma once

#include <cmath>


namespace geometry {

// Перекрывают ли отрезки друг друга. Не учитывает касание по точке.
bool overlap(double x1_l, double x1_r, double x2_l, double x2_r) {
    double l = std::max(x1_l, x2_l);
    double r = std::min(x1_r, x2_r);
    return l < r;
}

}
