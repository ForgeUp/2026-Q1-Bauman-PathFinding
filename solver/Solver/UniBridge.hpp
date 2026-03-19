#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Mix/Mix.hpp"
#include "../GridGenerator/Uniform/Uniform.hpp"
#include "../GridGenerator/Bridge/Bridge.hpp"
#include "../GridLinker/Knearest/Knearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using UniBridge = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Mix<
        GridGenerator::Uniform,
        GridGenerator::Bridge>::type,
    GridLinker::Knearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
