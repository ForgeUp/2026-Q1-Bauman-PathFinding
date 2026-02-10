#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace InitialGrider {

class ObstacleOffset : virtual public VarsBase {
public:
    ObstacleOffset(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {}
    
protected:
    void generate_initial_grid();

    int64_t enhance_nodes_count = stgs.enhance_rand_nodes_count;
    double  connection_radius   = stgs.connection_radius;
};

}


#include "generate_initial_grid.inl"
