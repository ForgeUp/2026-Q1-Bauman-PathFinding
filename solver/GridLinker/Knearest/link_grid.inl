#pragma once

#include "Knearest.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"

#include "kdtree/KDTree.hpp"


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

    Graph all; // Общий граф из вершин базового класса и вершин расширяющего класса.
    all.join_points(base);
    all.join_points(connect);

    Graph result(connect); // Итоговый граф.

    if (K < 1) return result; // Вырожденный случай: точки не соединяются.

    KDTree tree(all.verts); // KD-дерево для быстрого выявления ближайших соседей.
    
    int32_t limitK = 2 * K; // Предел количества соседей для рассмотрения. // [TODO] Перенести в сущность настроек.
    
    for (const auto& p : connect.verts) { // Цикл по вершинам дополняющего графа.

        std::vector<Point> neighbours; // Ближайшие соседи точки для связывания.
        neighbours = tree.nearest_k(p, check_collision ? limitK + 1 : K + 1); // +1 так как в графе гарантированно содержится точка для связывания. 

        for (int32_t collectK{0}; const auto& q : neighbours) { // Цикл по всем вершинам в порядке удаления от рассматриваемой вершины.
            if (p == q) continue; // Пропускаем точку, если она является собой.

            if (check_collision && S.collision({p,q})) continue; // Если включена проверка коллизии -> Пропускаем ребро, если оно не проходит на проверку на коллизию.
            // Иначе найдено подходящее ребро.

            result.add(p, q);
            collectK++;

            if (collectK >= K) break; // Остановка рассмотрения, если найдено необходимое число соседей.
        }
    }

    return result;
}
