#pragma once

#include <map>
#include <numeric>

#include "Knearest.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"

#include "geometry/dist.hpp"


/**
 * @brief
 *      Создаёт дополнение к базовому графу на основе вершин из расширяющего графа.
 *      Вершины расширяющего графа связываются с вершинами базового класса и между собой по методу K ближайших соседей.
 *      Не все вершины базового класса могут оказаться в итоговом графе.
 *      Изначальные рёбра расширяющего графа (при наличии) отбрасываются.
 * @param base
 *      Базовый граф.
 * @param connect
 *      Расширяющий граф.
 * @param K
 *      Количество ближайших вершин, с которыми связывается каждая вершина расширяющего графа.
 * @param check_collision
 *      Проверять ли ребро на коллизию с препятствиями при соединении.
 * @return
 *      Дополнение к базовому графу из рёбер и вершин, на которые опираются рёбра.
 */
template <typename Derived>
Graph GridLinker::Knearest<Derived>::link_grid(const Graph& base, const Graph& connect, int32_t K, bool check_collision) {
    auto& S = self();

    int32_t limitK = 2 * K; // Предел количества соседей для рассмотрения. // [TODO] Перенести в сущность настроек.

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

        std::vector<decltype(all.verts)::iterator> idxs(all.verts.size()); // Массив индексов вершин общего графа для сортировки относительно рассматриваемой вершины.
        int32_t pos = 0;
        for (auto it = all.verts.begin(); it != all.verts.end(); ++it) {
            idxs[pos++] = it;
        }
        std::sort(idxs.begin(), idxs.end(), [&all,&p](auto& l, auto& r){
            return geometry::dist(p, *l) < geometry::dist(p, *r); 
        });

        for (int32_t cntr{0}; const auto& idx : idxs) { // Цикл по всем вершинам в порядке удаления от рассматриваемой вершины.
            auto& q = *idx;
            
            if (p == q) continue; // Расстояние от точки до самой себя.

            if (dists.size() >= K || cntr++ >= limitK) break; // Остановка рассмотрения, если найдено необходимое число соседей или число попыток иссякло.

            if (check_collision && S.collision({p,q})) continue; // Проверка на коллизию по требованию.

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
