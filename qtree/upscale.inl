#pragma once

#include "qtree.hpp"

#include "types/Point.hpp"


Qtree::Box* Qtree::upscale(Box* b, const Point& p) {
    while (b->length() > min_box_size_point) {
        expand(b);
        b = locate(p, b);
    }
    return b;
};
