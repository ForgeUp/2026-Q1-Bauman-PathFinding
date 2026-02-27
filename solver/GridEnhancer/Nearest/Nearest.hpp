#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace GridEnhancer {

class Nearest : virtual public VarsBase {
public:
    Nearest(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {}
    
protected:
    bool enhance_graph();

protected:
    int64_t enhance_nodes_count = stgs.enhance_rand_nodes_count;
};

};


#include "enhance_graph.inl"
