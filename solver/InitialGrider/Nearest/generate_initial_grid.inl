#pragma once

#include "Nearest.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
void InitialGrider::Nearest::generate_initial_grid() {
    metric.time_in(__func__);

    Graph points = gridgen::lazy_points(stgs.initial_nodes_count, corner_min, corner_max);
    points.add(task.start);
    points.add(task.end);
    
    grid = gridgen::lazy_roads_Knearest(grid, points, stgs.nearest_count);
    
    visual.picture({task, sln, "initial_grid"});

    metric.time_out(__func__);
}
