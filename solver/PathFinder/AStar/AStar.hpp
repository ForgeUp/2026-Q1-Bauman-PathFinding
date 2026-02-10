#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace PathFinder {

class AStar : virtual public VarsBase {
public:
    AStar(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {}
    
protected:
    void find_path();
};

}


#include "find_path.inl"
