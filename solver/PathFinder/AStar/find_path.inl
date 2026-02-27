#pragma once

#include "Astar.hpp"

#include "pathfind/lazy.hpp"


template <typename Derived>
void PathFinder::AStar<Derived>::find_path() {
    auto& S = self();

    S.metric.time_in(__func__);

    auto res = pathfind::lazy(S.task, S.grid);

    S.path     = res.path;
    S.enhance  = Graph();
    S.examined = res.examined;
    S.invalid  = Graph();

    S.is_path_not_found = res.is_unreachable;

    S.metric.time_out(__func__);
}
