#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"

#include "qtree/Qtree.hpp"
using QTreeDS = Qtree;


namespace CollisionChecker {

class Qtree : virtual public VarsBase {
public:
    Qtree(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_), qtree(corner_min, corner_max) {}
    
protected:
    bool check_points_collision();
    bool check_edges_collision();

    QTreeDS qtree;
    bool qtree_ready{false};
};

};


#include "check_points_collision.inl"
#include "check_edges_collision.inl"
