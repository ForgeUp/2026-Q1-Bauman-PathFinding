#pragma once

#include "types/Point.hpp"


namespace geometry {

Point mid(const Point& p1, const Point& p2) {
    return Point(
        (p2.x + p1.x) * 0.5,
        (p2.y + p1.y) * 0.5
    );
}

}
