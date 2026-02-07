#pragma once

#include "qtree.hpp"

#include "geometry/overlap.hpp"

#include "math/eq.hpp"


bool Qtree::is_nearby(const Box* b1, const Box* b2) {
    auto& A = *b1;
    auto& B = *b2;
    if (math::eq(A.x_max, B.x_min) || math::eq(A.x_min, B.x_max))
        return geometry::overlap(A.y_min, A.y_max, B.y_min, B.y_max);

    if (math::eq(A.y_max, B.y_min) || math::eq(A.y_min, B.y_max))
        return geometry::overlap(A.x_min, A.x_max, B.x_min, B.x_max);

    return false;
}
