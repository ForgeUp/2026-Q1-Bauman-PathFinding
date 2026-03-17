#pragma once


namespace Grider {

template <typename Derived>
class AdaptiveRaise {
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    void generate_initial_grid();
    void enhance_graph();

protected:
    double multiplier{1};
    double rate{2};
};

}


#include "generate_initial_grid.inl"
#include "enhance_graph.inl"
