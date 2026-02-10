#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace GridEnhancer {

class Naive : virtual public VarsBase {
public:
    Naive(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {}
    
protected:
    bool enhance_graph();

protected:
    int64_t enhance_nodes_count = stgs.enhance_rand_nodes_count;
    double  connection_radius   = stgs.connection_radius;
};

};


#include "enhance_graph.inl"
