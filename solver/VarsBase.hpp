#pragma once

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"

#include "draw/Visualizer.hpp"


class VarsBase {
public:
    VarsBase(const Task& task_, const SolverSettings& stgs_) : task{task_}, stgs{stgs_} {}

protected:
    const Task& task;
    const SolverSettings& stgs;

    Solution sln;

    Graph& path     = sln.path;
    Graph& grid     = sln.grid;     // Граф, в котором выполняется поиск пути.
    Graph& enhance  = sln.enhance;  // Подграф добавленных вершин и ребёр.
    Graph& examined = sln.examined; // Подграф рассмотренных в процессе поиска пути дорог и вершин.
    Graph& invalid  = sln.invalid;  // Подграф, отброшенный из-за коллизии.

    Graph invalid_all;              // Подграф невалидных рёбер за все итерации основного цикла.
    Graph invalid_all_rand;         // Подграф невалидных рёбер за все итерации основного цикла, построенных на точках, сгенерированных случайно в пределах всей арены.

    int32_t attempts = 0;

    // Флаги.
    bool repeat    = true;
    bool terminate = false;
    bool is_found  = false;

    bool is_path_not_found = false;

    // Левый нижний и правый верхний углы арены.
    Point corner_min = Point(task.area.x_min, task.area.y_min);
    Point corner_max = Point(task.area.x_max, task.area.y_max);

protected:
    Visualizer visual;
};
