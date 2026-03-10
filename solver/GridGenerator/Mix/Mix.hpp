#pragma once

#include "types/Graph.hpp"

#include "solver/GridGenerator/Options.hpp"


namespace GridGenerator {

template <template<typename> class... Modules>
struct Mix {

    template <typename Derived>
    struct type : Modules<Derived>... {
        Derived& self() { return static_cast<Derived&>(*this); }

        Graph generate_grid(Options& opts);
    };
};

}


#include "generate_grid.inl"
