#pragma once

#include <set>

#include "Naive.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "geometry/is_inside.hpp"


template <typename Derived>
bool CollisionChecker::Naive<Derived>::collision(const Point& p) {
    auto& S = self();

    S.metric.count("point_collision_check_total");

    if (p.is_checked_collsn) p.is_collision;

    S.metric.count("point_collision_check_unique");

    p.is_collision = false;
    p.is_checked_collsn = true;

    for (auto& r : S.task.area.rocks) {
        if (!geometry::is_inside(p, r)) continue;
        p.is_collision = true;
        break;
    }

    return p.is_collision;
}
