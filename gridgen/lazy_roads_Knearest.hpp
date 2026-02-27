#pragma once

#include <map>

#include "types/Graph.hpp"
#include "types/Segment.hpp"

#include "geometry/dist.hpp"


namespace gridgen {

/**
 * @brief
 *      Создаёт новый граф, объединяя базовый и дополняющий графы.
 *      Вершины и рёбра базового класса выбираются все.
 *      Вершины дополняющего графа связываются с вершинами базового класса и между собой по методу K ближайших соседей.
 *      Изначальные рёбра дополняющего графа (при наличии) отбрасываются.
 * @param base
 *      Базовый граф.
 * @param enhance
 *      Дополнящий граф.
 * @param K
 *      Количество ближайших вершин, с которыми связывается вершина дополняющего графа.
 */
Graph lazy_roads_Knearest(const Graph& base, const Graph& enhance, int32_t K) {
    // Общий граф из вершин и рёбер базового класса и вершин расширяющего класса.
    Graph result(base);
    result.join_points(enhance);
    
    if (K < 1) return result; // Вырожденный случай: точки не соединяются.

    for (auto it = enhance.verts.begin(); it != enhance.verts.end(); ++it) { // Цикл по вершинам дополняющего графа.
        auto& p{*it};
        
        std::multimap<double,Point> dists;

        for (auto jt = result.verts.begin(); jt != result.verts.end(); ++jt) { // Цикл по всем вершинами.
            auto& q{*jt};

            if (p == q) continue; // Расстояние от точки до самой себя.

            double dist_pq = geometry::dist(p, q);

            if (dists.size() < K) {
                dists.emplace(dist_pq, q);
                continue;
            }

            auto furthest_it = std::prev(dists.end());
            auto& [dist_furthest, furthest_point] = *furthest_it;

            if (dist_pq < dist_furthest) {
                dists.erase(furthest_it);
                dists.emplace(dist_pq, q);
            }
        }

        for (auto& [dist, q] : dists) {
            result.add(p, q);
        }
    }

    return result;
}

}
