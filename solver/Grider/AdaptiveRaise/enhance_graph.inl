#pragma once

#include "AdaptiveRaise.hpp"

#include "types/Graph.hpp"

#include "solver/GridGenerator/Options.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


// Усиление графа путём уплотнения сетки случайными точками с удвоением количества на каждом шаге.
template <typename Derived>
void Grider::AdaptiveRaise<Derived>::enhance_graph() {
    auto& S = self();

    S.visual.picture({S.task, {.invalid = S.invalid}, "invalid"});
    S.invalid.clear();

    GridGenerator::Options opts = {
        .lazy = S.stgs.lazy,
        .connect = false,
        .nodes_count = static_cast<int32_t>(S.stgs.initial_nodes_count * multiplier)
    };
    Graph points = S.generate_grid(opts);

    Graph enhance = gridgen::lazy_roads_Knearest(S.grid, points, S.stgs.nearest_count);
    S.grid.join(enhance);

    // Увеличение количества точек для следующей итерации.
    multiplier *= rate;

    S.visual.picture({S.task, {.grid = S.grid,.enhance = enhance}, "enhanced_grid"});
}
