#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


void CollisionChecker::Qtree::build_qtree() {
    metric.time_in(__func__);

    for (const auto& r : task.area.rocks) {
        qtree.add(r);
    }
    qtree_ready = true;

    visual.picture({task, {.qtree = qtree}, "qtree"});
    
    metric.time_out(__func__);
}
