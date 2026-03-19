#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Uniform/Uniform.hpp"
#include "../GridLinker/Knearest/Knearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Uniform = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Uniform, 
    GridLinker::Knearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
