#pragma once


namespace GridEnhancer {

template <typename Derived>
class Nearest {
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool enhance_graph();

protected:
    bool is_init{0};
    int64_t enhance_nodes_count{0};
};

}


#include "enhance_graph.inl"
