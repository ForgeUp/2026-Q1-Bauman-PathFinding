#pragma once

#include "Naive.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
void InitialGrider::Naive::generate_initial_grid() {
    Graph points = gridgen::lazy_points(stgs.initial_nodes_count, corner_min, corner_max);
    points.add(task.start);
    points.add(task.end);
    
    grid = gridgen::lazy_roads(points, stgs.connection_radius);
    
    visual.picture({task, sln, "initial_grid"});
}
