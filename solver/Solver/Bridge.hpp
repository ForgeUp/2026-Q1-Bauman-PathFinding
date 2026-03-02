#pragma once

#include "../Solver.hpp"

#include "../InitialGrider/Bridge/Bridge.hpp"
#include "../GridEnhancer/Nearest/Nearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Bridge = Solver<
    InitialGrider::Bridge,
    GridEnhancer::Nearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
