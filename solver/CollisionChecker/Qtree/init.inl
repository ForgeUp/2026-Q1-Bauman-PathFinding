#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Task.hpp"
#include "types/Graph.hpp"


template <typename Derived>
void CollisionChecker::Qtree<Derived>::init() {
    auto& S = self();

    if (is_init) return;

    decltype(qtree) temp(S.corner_min, S.corner_max);
    qtree = std::move(temp);

    for (const auto& r : S.task.area.rocks) {
        qtree.add(r);
    }
    is_init = true;

    S.visual.picture({no_rocks(S.task), {.qtree = qtree}, "qtree"});
}
