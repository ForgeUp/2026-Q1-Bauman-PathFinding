#pragma once

#include "VarsBase.hpp"

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"

#include "draw.hpp"


template <typename... Bases>
class Compose :
    virtual public VarsBase,
    public Bases...
{
public:
    Compose(const Task& task, const SolverSettings& stgs) :
        VarsBase(task, stgs),
        Bases(task, stgs)...
    {}
};


template <typename Impl>
class BaseSolver : 
    virtual public VarsBase,
    public Impl
{
public:
    BaseSolver(const Task& task, const SolverSettings& stgs) :
        VarsBase(task, stgs),
        Impl(task, stgs)
    {}

    Solution run();

protected:
    using Impl::sln;
    using Impl::task;
    using Impl::repeat;
    using Impl::is_found;
    using Impl::terminate;
    using Impl::invalid_all;
    using Impl::generate_initial_grid;
    using Impl::find_path;
    using Impl::enhance_graph;
    using Impl::check_points_collision;
    using Impl::check_edges_collision;

    bool check_enchancement_limit();
};


template <typename Impl>
Solution BaseSolver<Impl>::run() {
    draw(task, sln, "initial");

    generate_initial_grid();
    
    while (repeat && !is_found) {

        find_path();

        // Завершение алгоритма, если в очередной раз не удалось найти путь и иссякло число попыток для поиска.
        terminate = check_enchancement_limit();
        if (terminate) break;

        // Дополнение графа узлами и дорогами, если путь не удалось найти.
        repeat = enhance_graph();
        if (repeat) continue;
        
        // Выявление и удаление коллидирующих точек.
        repeat = check_points_collision();
        if (repeat) continue;

        // Выявление и удаление коллидирующих рёбер.
        repeat = check_edges_collision();
        if (repeat) continue;

        // Иначе путь валиден и алгоритм заканчивают свою работу.
        is_found = true;
    }

    sln.is_fail = !is_found;
    // draw(task, sln, "final");

    draw(task, {.invalid_all = invalid_all}, "invalid_all");

    return sln;
}


template <typename Impl>
bool BaseSolver<Impl>::check_enchancement_limit() {
    if (!is_path_not_found) return false;
    if (attempts < stgs.enhance_attempts_limit) return false;

    return true;
}
