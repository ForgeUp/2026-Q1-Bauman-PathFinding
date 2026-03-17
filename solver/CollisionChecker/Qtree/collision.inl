#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


template <typename Derived>
bool CollisionChecker::Qtree<Derived>::collision(const Point& p) {
    auto& S = self();

    bool result = qtree.collision(p);

    return result;
}
