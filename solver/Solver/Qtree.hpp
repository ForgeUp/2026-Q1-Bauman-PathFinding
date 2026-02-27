#pragma once

#include "../Solver.hpp"

#include "../InitialGrider/Nearest/Nearest.hpp"
#include "../GridEnhancer/Nearest/Nearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Qtree = Solver<
    InitialGrider::Nearest,
    GridEnhancer::Nearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
