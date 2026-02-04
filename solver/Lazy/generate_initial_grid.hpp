#pragma once

#include "Lazy.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"

#include "draw.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
void Lazy::generate_initial_grid() {
    Graph points = gridgen::lazy_points(stgs.initial_nodes_count, corner_min, corner_max);
    points.add(task.start);
    points.add(task.end);
    
    grid = gridgen::lazy_roads(points, stgs.connection_radius);
    
    draw(task, sln, "initial_grid");
}
