#pragma once

#include "../BaseSolver.hpp"

#include "../InitialGrider/Naive/Naive.hpp"
#include "../GridEnhancer/Naive/Naive.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Qtree = BaseSolver<Compose<
    InitialGrider::Naive,
    GridEnhancer::Naive,
    CollisionChecker::Qtree,
    PathFinder::AStar
>>;

}
