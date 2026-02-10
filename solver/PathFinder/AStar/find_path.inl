#pragma once

#include "Astar.hpp"

#include "pathfind/lazy.hpp"


void PathFinder::AStar::find_path() {
    auto res = pathfind::lazy(task, grid);

    path     = res.path;
    enhance  = Graph();
    examined = res.examined;
    invalid  = Graph();

    is_path_not_found = res.is_unreachable;
}
