#pragma once

#include "./VarsBase.hpp"


class Lazy : public VarsBase {
public:
    Lazy(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {};

protected:
    void generate_initial_grid();
    void find_path();
    bool check_enchancement_limit();
    bool enhance_graph();
    bool check_points_collision();
    bool check_edges_collision();
};


#include "generate_initial_grid.hpp"
#include "find_path.hpp"
#include "check_enchancement_limit.hpp"
#include "enhance_graph.hpp"
#include "check_points_collision.hpp"
#include "check_edges_collision.hpp"
