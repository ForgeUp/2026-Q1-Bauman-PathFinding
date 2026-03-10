#pragma once

#include "Mix.hpp"

#include "solver/GridGenerator/Options.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <template<typename> class... Modules>
template <typename Derived>
Graph GridGenerator::Mix<Modules...>::type<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    S.metric.time_in(__func__);
    
    Graph mix;

    GridGenerator::Options subopts {
        .lazy = opts.lazy,
        .connect = false,
        .nodes_count = static_cast<int32_t>(opts.nodes_count * 0.5)
    };

    (mix.join(static_cast<Modules<Derived>&>(S).generate_grid(subopts)), ...);

    mix = gridgen::lazy_roads_Knearest({}, mix, S.stgs.nearest_count);

    if (opts.connect && opts.lazy) {
        mix = gridgen::lazy_roads_Knearest({}, mix, S.stgs.nearest_count);
    } else if (opts.connect && !opts.lazy) {
        mix = gridgen::lazy_roads_Knearest({}, mix, S.stgs.nearest_count); // [TODO] Заменить на вариант с проверкой коллизии.
    }

    S.metric.time_out(__func__);

    return mix;
}
