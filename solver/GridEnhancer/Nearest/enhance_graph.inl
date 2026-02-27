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
    Graph enhance_rand = gridgen::lazy_points(enhance_nodes_count, S.corner_min, S.corner_max);

    S.enhance.join(enhance_rand);

    // Соединение рёбрами новых точек с уже имеющимися.
    S.grid = gridgen::lazy_roads_Knearest(S.grid, enhance_rand, S.stgs.nearest_count);
    
    // Удаление рёбер, коллизия с которыми уже была установлена.
    for (const auto& s : S.invalid_all.edges()) {
        S.grid.remove(s);
    }

    // Увеличение количества точек для следующей итерации.
    enhance_nodes_count *= 2;

    S.visual.picture({S.task, S.sln, "point_enhancement"});

    S.metric.time_out(__func__);

    S.attempts++;
    return true;
}
