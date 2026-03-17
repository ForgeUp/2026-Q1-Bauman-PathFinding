#pragma once

#include "Astar.hpp"

#include "pathfind/lazy.hpp"


template <typename Derived>
bool PathFinder::AStar<Derived>::find_path() {
    auto& S = self();

    auto res = pathfind::lazy(S.task, S.grid);

    S.path     = res.path;
    S.examined = res.examined;

    return !res.is_unreachable;
}
