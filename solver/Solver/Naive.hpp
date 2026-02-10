#pragma once

#include "../BaseSolver.hpp"

#include "../InitialGrider/Naive/Naive.hpp"
#include "../GridEnhancer/Naive/Naive.hpp"
#include "../CollisionChecker/Naive/Naive.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Naive = BaseSolver<Compose<
    InitialGrider::Naive,
    GridEnhancer::Naive,
    CollisionChecker::Naive,
    PathFinder::AStar
>>;

}
