#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Mix/Mix.hpp"
#include "../GridGenerator/Uniform/Uniform.hpp"
#include "../GridGenerator/Gauss/Gauss.hpp"
#include "../GridLinker/Knearest/Knearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using UniGauss = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Mix<
        GridGenerator::Uniform,
        GridGenerator::Gauss>::template type,
    GridLinker::Knearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
