#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


template <typename Derived>
void CollisionChecker::Qtree<Derived>::build_qtree() {
    auto& S = self();

    S.metric.time_in(__func__);

    decltype(qtree) temp(S.corner_min, S.corner_max);
    qtree = std::move(temp);

    for (const auto& r : S.task.area.rocks) {
        qtree.add(r);
    }
    is_init = true;

    S.visual.picture({S.task, {.qtree = qtree}, "qtree"});
    
    S.metric.time_out(__func__);
}
