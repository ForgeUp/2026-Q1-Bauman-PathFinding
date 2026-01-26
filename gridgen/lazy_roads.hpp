#pragma once

#include <vector>
#include <map>

#include "types/Graph.hpp"
#include "types/Segment.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"

#include "geometry/dist.hpp"


namespace gridgen {
    
// Генерация дорог БЕЗ проверки пересечения с препятствиями.
Graph lazy_roads(const Task& task, Graph& points, const SolverSettings& stgs) {
    Graph graph;

    for (auto it = points.verts.begin(); it != points.verts.end(); ++it) {
        for (auto jt = std::next(it); jt != points.verts.end(); ++jt) {
            auto& p{*it};
            auto& q{*jt};

            if (geometry::dist(p, q) < stgs.connection_radius) graph.add(p, q);
        }
    }

    return graph;
}

}
