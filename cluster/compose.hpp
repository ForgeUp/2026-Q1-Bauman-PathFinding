#pragma once

#include <cmath>
#include <vector>
#include <unordered_map>

#include "types/Rock.hpp"

#include "geometry/intersect.hpp"


namespace cluster {

struct Cluster {
    std::vector<Rock> rocks;
};

std::vector<Cluster> compose(const std::vector<Rock>& rs) {
    std::vector<int64_t> marks(rs.size(), 0);     // Метки кластеров.
    std::vector<int64_t> proxy(rs.size() * 2, 0); // Ретрансляция меток кластеров при объединении.
    int64_t free_mark = 1;

    for (size_t i = 0; i < proxy.size(); ++i) {
        proxy[i] = i;
    }

    // Простановка меток кластеров.
    for (size_t i = 0; i < rs.size(); ++i) {
        auto& r1 = rs[i];

        for (size_t j = i + 1; j < rs.size(); ++j) {
            auto& r2 = rs[j];

            if (!geometry::intersect(r1, r2)) continue;

            if (marks[i] == 0 && marks[j] == 0) { // r1 и r2 ещё не в кластере -> создаём новый кластер.
                proxy[free_mark] = free_mark;
                marks[i] = marks[j] = free_mark;
                free_mark++;
            } else if (marks[i] == 0) {          // Только r1 не в кластере -> присоединяем r1 к кластеру r2.
                marks[i] = proxy[marks[j]];
            } else if (marks[j] == 0) {          // Только r2 не в кластере -> присоединяем r2 к кластеру r1.
                marks[j] = proxy[marks[i]];
            } else {                             // r1 и r2 уже в кластерах -> объединяем кластеры.
                int64_t mark_i_root = proxy[marks[i]];
                int64_t mark_j_root = proxy[marks[j]];
                
                if (mark_i_root != mark_j_root) {
                    // Объединяем: меньший корень становится родителем.
                    int64_t new_root = std::min(mark_i_root, mark_j_root);
                    int64_t old_root = std::max(mark_i_root, mark_j_root);
                    
                    // Обновляем proxy для всех меток.
                    for (size_t k = 0; k < proxy.size(); ++k) {
                        if (proxy[k] == old_root) {
                            proxy[k] = new_root;
                        }
                    }
                }
            }
        }

        // Если препятствие ни с чем не пересекается, создаем кластер из одного элемента.
        if (marks[i] == 0) {
            marks[i] = free_mark;
            proxy[free_mark] = free_mark;
            free_mark++;
        }
    }

    // Нормализация меток.
    for (size_t i = 0; i < rs.size(); ++i) {
        if (marks[i] != 0) {
            marks[i] = proxy[marks[i]];
        }
    }

    // Распределение фигур по кластерам.
    std::unordered_map<int64_t, Cluster> cluster_map;
    for (size_t i = 0; i < rs.size(); ++i) {
        if (marks[i] != 0) {
            cluster_map[marks[i]].rocks.push_back(rs[i]);
        }
    }

    // Преобразование в вектор.
    std::vector<Cluster> clusters;
    clusters.reserve(cluster_map.size());
    for (auto& [_, cluster] : cluster_map) {
        clusters.push_back(std::move(cluster));
    }

    return clusters;
}

}
