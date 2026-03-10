#pragma once

#include "types/Graph.hpp"

#include "solver/GridGenerator/Options.hpp"


namespace GridGenerator {

template <typename Derived>
class ObstacleOffset {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    Graph generate_grid(Options& opts);
};

}


#include "generate_grid.inl"
