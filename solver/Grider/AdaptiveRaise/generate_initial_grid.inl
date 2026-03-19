#pragma once

#include "AdaptiveRaise.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "solver/GridGenerator/Options.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация изначальной маршрутной сети без проверки коллизии с препятствиями.
template <typename Derived>
void Grider::AdaptiveRaise<Derived>::generate_initial_grid() {
    auto& S = self();

    GridGenerator::Options opts = {
        .check_collision = !S.stgs.lazy,
        .connect = true,
        .nodes_count = S.stgs.initial_nodes_count
    };
    S.grid = S.generate_grid(opts);

    Graph points;
    points.add(S.task.start);
    points.add(S.task.end);
    
    Graph enhance = gridgen::lazy_roads_Knearest(S.grid, points, S.stgs.nearest_count);
    S.grid.join(enhance);

    S.visual.picture({S.task, S.sln, "initial_grid"});
}
