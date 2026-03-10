#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Bridge/Bridge.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Bridge = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Bridge, 
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
