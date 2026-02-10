#pragma once

#include "../BaseSolver.hpp"

#include "../InitialGrider/ObstacleOffset/ObstacleOffset.hpp"
#include "../GridEnhancer/Naive/Naive.hpp"
#include "../CollisionChecker/Naive/Naive.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using ObstacleOffset = BaseSolver<Compose<
    InitialGrider::ObstacleOffset,
    GridEnhancer::Naive,
    CollisionChecker::Naive,
    PathFinder::AStar
>>;

}
