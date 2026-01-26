#pragma once

#include "types/Graph.hpp"
#include "types/Segment.hpp"

#include "geometry/dist.hpp"


namespace gridgen {
    
// Генерация дорог БЕЗ проверки пересечения с препятствиями.
Graph lazy_roads(Graph& points, double connection_radius) {
    Graph graph;

    for (auto it = points.verts.begin(); it != points.verts.end(); ++it) {
        for (auto jt = std::next(it); jt != points.verts.end(); ++jt) {
            auto& p{*it};
            auto& q{*jt};

            if (geometry::dist(p, q) < connection_radius) graph.add(p, q);
        }
    }

    return graph;
}

}
