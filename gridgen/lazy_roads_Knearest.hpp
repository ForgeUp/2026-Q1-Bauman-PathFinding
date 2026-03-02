#pragma once

#include <map>

#include "types/Graph.hpp"
#include "types/Segment.hpp"

#include "geometry/dist.hpp"


namespace gridgen {

/**
 * @brief
 *      Создаёт дополнение к базовому графу на основе вершин из расширяющего графа.
 *      Вершины расширяющего графа связываются с вершинами базового класса и между собой по методу K ближайших соседей.
 *      Не все вершины базового класса могут оказаться в итоговом графе.
 *      Изначальные рёбра расширяющего графа (при наличии) отбрасываются.
 * @param base
 *      Базовый граф.
 * @param enhance
 *      Расширяющий граф.
 * @param K
 *      Количество ближайших вершин, с которыми связывается каждая вершина расширяющего графа.
 * @return
 *      Дополнение к базовому графу из рёбер и вершин, на которые опираются рёбра.
 */
Graph lazy_roads_Knearest(const Graph& base, const Graph& connect, int32_t K) {
    // Общий граф из вершин базового класса и вершин расширяющего класса.
    Graph all;
    all.join_points(base);
    all.join_points(connect);

    // Итоговый граф.
    Graph result(connect);
    
    if (K < 1) return result; // Вырожденный случай: точки не соединяются.

    for (auto it = connect.verts.begin(); it != connect.verts.end(); ++it) { // Цикл по вершинам дополняющего графа.
        auto& p{*it};
        
        std::multimap<double,Point> dists;

        for (auto jt = all.verts.begin(); jt != all.verts.end(); ++jt) { // Цикл по всем вершинам.
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
