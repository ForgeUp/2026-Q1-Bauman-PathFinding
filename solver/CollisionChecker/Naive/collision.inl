#pragma once

#include <set>

#include "Naive.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


template <typename Derived>
bool CollisionChecker::Naive<Derived>::collision(const Point& p) {
    auto& S = self();

    // [TODO] Добавить отметку о проверки коллизии.
    for (auto& r : S.task.area.rocks) {
        if (!geometry::is_inside(p, r)) continue;
        return true;
    }

    return false;
}
