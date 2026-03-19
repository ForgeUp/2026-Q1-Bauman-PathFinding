#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Mix/Mix.hpp"
#include "../GridGenerator/Uniform/Uniform.hpp"
#include "../GridGenerator/ClusterOffset/ClusterOffset.hpp"
#include "../GridLinker/Knearest/Knearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using ClusterOffset = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Mix<
        GridGenerator::Uniform,
        GridGenerator::ClusterOffset>::type,
    GridLinker::Knearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
