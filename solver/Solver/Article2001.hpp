#pragma once

#include "../Solver.hpp"

#include "../Grider/AdaptiveRaise/AdaptiveRaise.hpp"
#include "../GridGenerator/Mix/Mix.hpp"
#include "../GridGenerator/Uniform/Uniform.hpp"
#include "../GridGenerator/Article2001/Article2001.hpp"
#include "../GridLinker/Knearest/Knearest.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Article2001 = Solver<
    Grider::AdaptiveRaise,
    GridGenerator::Mix<
        GridGenerator::Uniform,
        GridGenerator::Article2001>::type,
    GridLinker::Knearest,
    CollisionChecker::Qtree,
    PathFinder::AStar
>;

}
