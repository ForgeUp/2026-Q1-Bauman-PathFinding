#pragma once

#include "../Solver.hpp"

#include "../InitialGrider/Naive/Naive.hpp"
#include "../GridEnhancer/Naive/Naive.hpp"
#include "../CollisionChecker/Naive/Naive.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Naive = Solver<
    InitialGrider::Naive,
    GridEnhancer::Naive,
    CollisionChecker::Naive,
    PathFinder::AStar
>;

}
