#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
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

template <typename Derived>
bool CollisionChecker::Qtree<Derived>::collision(const Segment& s) {
    auto& S = self();
    
    S.metric.count("edge_collision_check_total");

    auto& p1 = s.p1 < s.p2 ? s.p1 : s.p2; 
    auto& p2 = s.p1 < s.p2 ? s.p2 : s.p1;
    Segment e{p1, p2};

    if (edge_cache.contains(e)) return edge_cache[e];

    S.metric.count("edge_collision_check_unique");
    
    edge_cache[e] = qtree.collision(e);

    return edge_cache[e];
}
