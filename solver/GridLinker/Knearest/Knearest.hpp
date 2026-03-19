#pragma once

#include "types/Graph.hpp"


namespace GridLinker {

template <typename Derived>
class Knearest {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    Graph link_grid(const Graph& base, const Graph& connect, int32_t K, bool check_collision);
};

}


#include "link_grid.inl"
