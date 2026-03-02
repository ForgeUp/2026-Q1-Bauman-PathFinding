#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


template <typename Derived>
bool CollisionChecker::Qtree<Derived>::collision(const Point& p) {
    auto& S = self();

    if (!is_init) build_qtree();

    S.metric.time_in(__func__);

    bool result = qtree.collision(p);
    
    S.metric.time_out(__func__);

    return result;
}
