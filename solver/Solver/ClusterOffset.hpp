#pragma once

#include "../BaseSolver.hpp"

#include "../InitialGrider/ClusterOffset/ClusterOffset.hpp"
#include "../GridEnhancer/Naive/Naive.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using ClusterOffset = BaseSolver<Compose<
    InitialGrider::ClusterOffset,
    GridEnhancer::Naive,
    CollisionChecker::Qtree,
    PathFinder::AStar
>>;

}
