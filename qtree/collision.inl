#pragma once

#include "qtree.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"

#include "geometry/intersect.hpp"
#include "geometry/on_segment.hpp"


bool Qtree::collision(const Point& p) {
    Box* b = locate(p);
    if (b == nullptr) return false;
    if (b->type == Type::Free) return false;
    if (b->type == Type::Busy) return true;
    // Иначе b->type == Type::Mix.
    for (const auto& [obst, seg_idxs] : b->collisions) {
        for (const auto& idx : seg_idxs) {
            if (geometry::on_segment(p, get_segment_by_idx(obst, idx))) return true;
        }
    }
    return false;
}

bool Qtree::collision(const Segment& s) {
    std::vector<Box*> trail = trace<false>(s);
    for (const auto& b : trail) {
        if (b->type == Type::Free || b->type == Type::Undefined) continue;
        if (b->type == Type::Busy) return true;
        // Иначе b->type == Type::Mix.
        for (const auto& [obst, seg_idxs] : b->collisions) {
            for (const auto& idx : seg_idxs) {
                if (geometry::intersect(s, get_segment_by_idx(obst, idx))) {
                    return true;
                }
            }
        }
    }
    return false;
}
