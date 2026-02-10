#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace InitialGrider {

class Naive : virtual public VarsBase {
public:
    Naive(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {}
    
protected:
    void generate_initial_grid();
};

}


#include "generate_initial_grid.inl"
