#pragma once

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"
#include "types/Services.hpp"

#include "draw/VisualizerAdapter.hpp"

#include "utils/Timer.hpp"


template <template<typename> class... Modules>
class SolverBase: public Modules<SolverBase<Modules...>>... {
public:
    using Self = SolverBase<Modules...>;

protected:
    Self& self() { return *this; }

public:
    SolverBase(const Task& task_, const SolverSettings& stgs_, const Services& srvs_) : task(task_), stgs(stgs_), srvs(srvs_) {}

    Solution run();

public:
    const Task& task;
    const SolverSettings& stgs;

    Solution sln;

    Graph& path     = sln.path;
    Graph& grid     = sln.grid;     // Граф, в котором выполняется поиск пути.
    Graph& examined = sln.examined; // Подграф рассмотренных в процессе поиска пути дорог и вершин.
    Graph& invalid  = sln.invalid;  // Подграф, отброшенный из-за коллизии.

    Graph invalid_all;              // Подграф невалидных рёбер за все итерации основного цикла.
    Graph invalid_all_rand;         // Подграф невалидных рёбер за все итерации основного цикла, построенных на точках, сгенерированных случайно в пределах всей арены.

    int32_t attempts{0};            // Счётчик выполненных усилений графа. 

    // Левый нижний и правый верхний углы арены.
    Point corner_min = Point(task.area.x_min, task.area.y_min);
    Point corner_max = Point(task.area.x_max, task.area.y_max);

public:
    Services srvs;
    VisualizerAdapter& visual = srvs.visual;
    
    Metric& metric = sln.metric;

private:
    void init();
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

    Timer total;

    auto metric_time = [&](const std::string& name, auto&& func) -> decltype(auto) {
        S.metric.time_in(name);

        if constexpr (std::is_void_v<std::invoke_result_t<decltype(func)>>) {
            func();
            S.metric.time_out(name);
        } else{
            auto result = func();
            S.metric.time_out(name);
            return result;
        }
    };

    std::map<std::string,std::string> names = {
        {"S.init()", "Инициализация модулей"},
        {"S.generate_initial_grid()", "Генерация изначальной PRM"},
        {"S.find_path()", "Поиск пути"},
        {"S.check_path_collision()", "Проверка пути на коллизию"},
        {"S.enhance_graph()", "Усиление PRM"}
    };

    #define METRIC_CALL(expr) metric_time(names[#expr], [&]{ return (expr); })
    
    for (const auto& [_, name] : names) {
        S.metric.time_reg(name);
    }

    S.visual.picture({S.task, S.sln, "initial"});
    
    bool is_found_unchecked = false;
    bool is_valid_found     = false;
    bool is_invalid         = false;

    METRIC_CALL(S.init());                                        // Инициализация модулей.
    METRIC_CALL(S.generate_initial_grid());                       // Генерация изначальной маршрутной сети.

    while (!is_valid_found) {                                     // Основной цикл алгоритма.
        is_found_unchecked = METRIC_CALL(S.find_path());          // Поиск пути в маршрутной сети.
        
        if (is_found_unchecked) {                                 // Путь удалось найти.
            is_invalid = METRIC_CALL(S.check_path_collision());   // - Проверка найденного пути на коллизию.
            if (is_invalid) continue;                             // - Путь невалиден -> Путь ищется заново.
            is_valid_found = true; break;                         // - Путь валиден -> Алгоритм заканчивает свою работу.
        } else {                                                  // Иначе путь найти не удалось.
            if (attempts >= stgs.enhance_attempts_limit) {        // - Завершение алгоритма, если иссякло число попыток для усиления.
                is_valid_found = false; break; 
            }
            METRIC_CALL(S.enhance_graph());                       // - Дополнение графа узлами и дорогами.
            attempts++;
        }
    }

    S.sln.is_fail = !is_valid_found;
    
    S.invalid.clear();
    S.visual.picture({S.task, {.invalid_all = S.invalid_all}, "invalid_all"});
    S.visual.picture({S.task, S.sln, "result"});

    #undef METRIC_CALL

    S.metric.total = total.tick();

    return S.sln;
}


template <typename T>
concept HasInit = requires(T t) { t.init(); };

template <template<typename> class... Modules>
void SolverBase<Modules...>::init() {
    auto& S = self();

    ([&]{
        using M = Modules<Self>;
        if constexpr (HasInit<M>) static_cast<M&>(S).init();
    }(), ...);
}
