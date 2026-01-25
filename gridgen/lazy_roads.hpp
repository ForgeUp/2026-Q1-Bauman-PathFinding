#pragma once

#include <vector>
#include <map>

#include "types/Segment.hpp"
#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace gridgen {

using square_idx = std::pair<int64_t,int64_t>;
    
// Генерация дорог БЕЗ проверки пересечения с препятствиями.
Graph lazy_roads(const Task& task, std::map<square_idx,Point> squares, const SolverSettings& stgs) {
    Graph graph;

    // Перебираем ячейки с левой нижней и пытаемся соединить точки в соседних ячейках.
    for (int64_t i = 0; i < stgs.squares_per_side; ++i) { // Цикл вдоль оси Y.
        for (int64_t j = 0; j < stgs.squares_per_side; ++j) { // Цикл вдоль оси X.
            square_idx curr_square_idx{i,j}; 
            
            // Пропускаем недоступные ячейки.
            if (!squares.contains(curr_square_idx)) continue;

            auto& curr_point = squares[curr_square_idx];

            // Проверяем ячейки справа, справа-сверху, сверху, слева-сверху на доступность.
            for (auto near_square_idx : std::vector<square_idx>{{i,j+1}, {i+1,j+1}, {i+1,j}, {i+1,j-1}}) {
                // Пропускаем недоступные ячейки.
                if (!squares.contains(near_square_idx)) continue;

                auto& near_point = squares[near_square_idx];
                graph.add(curr_point, near_point);
            }
        }
    }

    return graph;
}

}
