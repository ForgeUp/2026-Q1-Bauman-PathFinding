#pragma once

#include <cmath>

#include "Nearest.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads_Knearest.hpp"


// Усиление графа путём уплотнения сетки случайными точками с удвоения количества на каждом шаге.
bool GridEnhancer::Nearest::enhance_graph() {
    if (!is_path_not_found) return false;
    
    metric.time_in(__func__);

    // Генерация случайных точек в пределах всей арены.
    Graph enhance_rand = gridgen::lazy_points(enhance_nodes_count, corner_min, corner_max);

    enhance.join(enhance_rand);

    // Соединение рёбрами новых точек с уже имеющимися.
    grid = gridgen::lazy_roads_Knearest(grid, enhance_rand, stgs.nearest_count);
    
    // Удаление рёбер, коллизия с которыми уже была установлена.
    for (const auto& s : invalid_all.edges()) {
        grid.remove(s);
    }

    // Увеличение количества точек для следующей итерации.
    enhance_nodes_count *= 2;

    visual.picture({task, sln, "point_enhancement"});

    metric.time_out(__func__);

    attempts++;
    return true;
}
