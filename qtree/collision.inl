#pragma once

#include "qtree.hpp"

#include "geometry/on_segment.hpp"


bool Qtree::collision(const Point& p) {
    Box* b = locate(p);
    if (b == nullptr) return false;
    if (b->type == Type::Free) return false;
    if (b->type == Type::Busy) return true;
    // Иначе b->type == Type::Mix.
    for (const auto& s : b->segments) {
        if (geometry::on_segment(p,*s)) return true;
    }
    return false;
}
