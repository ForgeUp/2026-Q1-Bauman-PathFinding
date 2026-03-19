#pragma once

#include "Mix.hpp"

#include "solver/GridGenerator/Options.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <template<typename> class... Modules>
template <typename Derived>
Graph GridGenerator::Mix<Modules...>::type<Derived>::generate_grid(Options& opts) {
    auto& S = self();
    
    Graph mix;

    GridGenerator::Options subopts {
        .check_collision = opts.check_collision,
        .connect = false,
        .nodes_count = static_cast<int32_t>(opts.nodes_count * 0.5)
    };

    (mix.join(static_cast<Modules<Derived>&>(S).generate_grid(subopts)), ...);

    mix = S.link_grid({}, mix, S.stgs.nearest_count, opts.check_collision);

    return mix;
}
