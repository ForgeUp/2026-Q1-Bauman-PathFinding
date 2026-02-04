#pragma once

#include "./Lazy/Lazy.hpp"


class LazyNaiveGridSealing : public Lazy {
public:
    LazyNaiveGridSealing(const Task& task_, const SolverSettings& stgs_) : Lazy(task_, stgs_) {};

protected:
    int64_t enhance_nodes_count = stgs.enhance_rand_nodes_count;
    double  connection_radius   = stgs.connection_radius;
    bool enhance_graph();
};


#include "enhance_graph.hpp"
