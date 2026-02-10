#pragma once

#include "../BaseSolver.hpp"

#include "../InitialGrider/ClusterOffset/ClusterOffset.hpp"
#include "../GridEnhancer/Naive/Naive.hpp"
#include "../CollisionChecker/Naive/Naive.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using ClusterOffset = BaseSolver<Compose<
    InitialGrider::ClusterOffset,
    GridEnhancer::Naive,
    CollisionChecker::Naive,
    PathFinder::AStar
>>;

}
