#pragma once

#include "Naive.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <typename Derived>
void InitialGrider::Naive<Derived>::generate_initial_grid() {
    auto& S = self();

    S.metric.time_in(__func__);

    Graph points = gridgen::lazy_points(S.stgs.initial_nodes_count, S.corner_min, S.corner_max);
    points.add(S.task.start);
    points.add(S.task.end);
    
    S.grid = gridgen::lazy_roads(points, S.stgs.connection_radius);
    
    S.visual.picture({S.task, S.sln, "initial_grid"});

    S.metric.time_out(__func__);
}
