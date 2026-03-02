#pragma once

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"
#include "types/Services.hpp"

#include "draw/VisualizerAdapter.hpp"


template <template<typename> class... Modules>
class SolverBase: public Modules<SolverBase<Modules...>>... {
protected:
    SolverBase<Modules...>& self() { return *this; }

public:
    SolverBase(const Task& task_, const SolverSettings& stgs_, const Services& srvs_) : task(task_), stgs(stgs_), srvs(srvs_) {}

    Solution run();

public:
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

public:
    Services srvs;
    VisualizerAdapter& visual = srvs.visual;
    
    Metric& metric = sln.metric;

public:
    bool check_enchancement_limit();
};


template <template<typename> class... Modules>
class Solver {
private:
    SolverBase<Modules...> base;

public:
    Solver(const Task& task_, const SolverSettings& stgs_, const Services& srvs_) : base(task_, stgs_, srvs_) {}

    Solution run() {
        return base.run();
    }
};


template <template<typename> class... Modules>
Solution SolverBase<Modules...>::run() {
    auto& S = self();

    S.visual.picture({S.task, S.sln, "initial"});

    S.generate_initial_grid();
    
    while (S.repeat && !S.is_found) {

        S.find_path();

        // Завершение алгоритма, если в очередной раз не удалось найти путь и иссякло число попыток для поиска.
        S.terminate = S.check_enchancement_limit();
        if (S.terminate) break;

        // Дополнение графа узлами и дорогами, если путь не удалось найти.
        S.repeat = S.enhance_graph();
        if (S.repeat) continue;
        
        // Выявление и удаление коллидирующих точек.
        S.repeat = S.check_points_collision();
        if (S.repeat) continue;

        // Выявление и удаление коллидирующих рёбер.
        S.repeat = S.check_edges_collision();
        if (S.repeat) continue;

        // Иначе путь валиден и алгоритм заканчивают свою работу.
        S.is_found = true;
    }

    S.sln.is_fail = !S.is_found;
    
    S.invalid.clear();
    S.visual.picture({S.task, {.invalid_all = S.invalid_all}, "invalid_all"});
    S.visual.picture({S.task, S.sln, "result"});

    return S.sln;
}


template <template<typename> class... Modules>
bool SolverBase<Modules...>::check_enchancement_limit() {
    if (!is_path_not_found) return false;
    if (attempts < stgs.enhance_attempts_limit) return false;

    return true;
}
