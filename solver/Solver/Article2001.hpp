#pragma once

#include "../Solver.hpp"

#include "../InitialGrider/Nearest/Nearest.hpp"
#include "../GridEnhancer/Article2001/Article2001.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Article2001 = Solver<
    InitialGrider::Nearest,
    GridEnhancer::Article2001,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
