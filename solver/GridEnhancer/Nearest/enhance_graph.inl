#pragma once

#include <cmath>

#include "Nearest.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads_Knearest.hpp"


// Усиление графа путём уплотнения сетки случайными точками с удвоения количества на каждом шаге.
template <typename Derived>
bool GridEnhancer::Nearest<Derived>::enhance_graph() {
    auto& S = self();

    if (!S.is_path_not_found) return false;
    
    S.metric.time_in(__func__);
    
    if (!is_init) {
        enhance_nodes_count = S.stgs.enhance_rand_nodes_count;
        is_init = true;
    }

    // Генерация случайных точек в пределах всей арены.
    Graph enhance = gridgen::lazy_points(enhance_nodes_count, S.corner_min, S.corner_max);

    S.enhance.join(enhance);

    // Соединение рёбрами новых точек с уже имеющимися.
    enhance = gridgen::lazy_roads_Knearest(S.grid, enhance, S.stgs.nearest_count);
    S.grid.join(enhance);

    // Увеличение количества точек для следующей итерации.
    enhance_nodes_count *= 2;
    
    S.visual.picture({S.task, {.enhance = enhance}, "point_enhancement"});

    S.metric.time_out(__func__);

    S.attempts++;
    return true;
}
