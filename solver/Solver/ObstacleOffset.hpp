#pragma once

#include "../Solver.hpp"

#include "../InitialGrider/ObstacleOffset/ObstacleOffset.hpp"
#include "../GridEnhancer/Nearest/Nearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using ObstacleOffset = Solver<
    InitialGrider::ObstacleOffset,
    GridEnhancer::Nearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
