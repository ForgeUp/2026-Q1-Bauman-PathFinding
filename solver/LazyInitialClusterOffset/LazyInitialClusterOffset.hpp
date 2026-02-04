#pragma once

#include "./LazyNaiveGridSealing/LazyNaiveGridSealing.hpp"


class LazyInitialClusterOffset : public LazyNaiveGridSealing {
public:
    LazyInitialClusterOffset(const Task& task_, const SolverSettings& stgs_) : LazyNaiveGridSealing(task_, stgs_) {};

protected:
    void generate_initial_grid();
};


#include "generate_initial_grid.hpp"
