#pragma once


namespace InitialGrider {

template <typename Derived>
class ObstacleOffset {
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    void generate_initial_grid();

protected:
    bool is_init{false};
    int64_t enhance_nodes_count{0};
    double  connection_radius  {0};
};

}


#include "generate_initial_grid.inl"
