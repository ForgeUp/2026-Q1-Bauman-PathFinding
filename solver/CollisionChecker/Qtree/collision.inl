#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


template <typename Derived>
bool CollisionChecker::Qtree<Derived>::collision(const Point& p) {
    auto& S = self();

    S.metric.count("point_collision_check_total");

    if (p.is_checked_collsn) p.is_collision;
    
    S.metric.count("point_collision_check_unique");

    p.is_collision = qtree.collision(p);
    p.is_checked_collsn = true;

    return p.is_collision;
}
