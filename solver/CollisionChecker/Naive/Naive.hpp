#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace CollisionChecker {

class Naive : virtual public VarsBase {
public:
    Naive(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {}
    
protected:
    bool check_points_collision();
    bool check_edges_collision();
};

};


#include "check_points_collision.inl"
#include "check_edges_collision.inl"
