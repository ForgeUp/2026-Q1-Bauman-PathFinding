#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Mix/Mix.hpp"
#include "../GridGenerator/Uniform/Uniform.hpp"
#include "../GridGenerator/ObstacleBased/ObstacleBased.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using ObstacleBased = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Mix<
        GridGenerator::Uniform,
        GridGenerator::ObstacleBased>::type,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
