#pragma once

#include "../Solver.hpp"

#include "../InitialGrider/ClusterOffset/ClusterOffset.hpp"
#include "../GridEnhancer/Nearest/Nearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using ClusterOffset = Solver<
    InitialGrider::ClusterOffset,
    GridEnhancer::Nearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
