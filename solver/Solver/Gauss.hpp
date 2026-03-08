#pragma once

#include "../Solver.hpp"

#include "../InitialGrider/Gauss/Gauss.hpp"
#include "../GridEnhancer/Nearest/Nearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Gauss = Solver<
    InitialGrider::Gauss,
    GridEnhancer::Nearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
