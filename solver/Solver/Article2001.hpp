#pragma once

#include "../BaseSolver.hpp"

#include "../InitialGrider/Naive/Naive.hpp"
#include "../GridEnhancer/Article2001/Article2001.hpp"
#include "../CollisionChecker/Qtree/Qtree.hpp"
#include "../PathFinder/AStar/AStar.hpp"


namespace solver {

using Article2001 = BaseSolver<Compose<
    InitialGrider::Naive,
    GridEnhancer::Article2001,
    CollisionChecker::Qtree,
    PathFinder::AStar
>>;

}
