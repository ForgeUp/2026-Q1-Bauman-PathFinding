#pragma once

#include "Nearest.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <typename Derived>
void InitialGrider::Nearest<Derived>::generate_initial_grid() {
    auto& S = self();

    S.metric.time_in(__func__);

    Graph points = gridgen::lazy_points(S.stgs.initial_nodes_count, S.corner_min, S.corner_max);
    points.add(S.task.start);
    points.add(S.task.end);
    
    S.grid = gridgen::lazy_roads_Knearest(S.grid, points, S.stgs.nearest_count);
    
    S.visual.picture({S.task, S.sln, "initial_grid"});

    S.metric.time_out(__func__);
}
