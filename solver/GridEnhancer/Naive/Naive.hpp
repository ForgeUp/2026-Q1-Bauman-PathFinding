#pragma once


namespace GridEnhancer {

template <typename Derived>
class Naive {
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool enhance_graph();

protected:
    bool is_init{false};
    int64_t enhance_nodes_count{0};
    double  connection_radius  {0};
};

}


#include "enhance_graph.inl"
