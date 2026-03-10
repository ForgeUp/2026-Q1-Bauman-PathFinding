#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Gauss/Gauss.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Gauss = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Gauss, 
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
