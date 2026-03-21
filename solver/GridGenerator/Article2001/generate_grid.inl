#pragma once

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>
#include <ranges>

#include "Article2001.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


// Генерация случайных точек вокруг отброшенных рёбер.
template <typename Derived>
Graph GridGenerator::Article2001<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    S.visual.picture({S.task, {.invalid = S.invalid}, "invalid"});
    S.invalid.clear();
    
    std::random_device rd;
    std::mt19937 gen(rd());

    Graph result;
    
    std::vector<int32_t> idxs; // Набор уникальных случайных индексов рёбер, отброшенных за все итерации алгоритма, рёбер в порядке возрастания.
    int32_t samples_count = std::min(opts.nodes_count, S.invalid_all_rand.edges_count);
    
    idxs = std::invoke([&](int32_t start, int32_t end, int32_t count) {
        std::vector<int32_t> result(count);
        
        std::sample(
            std::views::iota(start, end).begin(),
            std::views::iota(start, end).end(),
            result.begin(), count, gen
        );

        std::sort(result.begin(), result.end());

        return result;
    }, 0, S.invalid_all_rand.edges_count, samples_count);

    for (int32_t i{0}, j{0}; const auto& s : S.invalid_all_rand.edges()) {
        if (j >= idxs.size()) break;
        if (i++ != idxs[j]) continue;
        j++;

        Point mid (
            (s.p1.x + s.p2.x) / 2,
            (s.p1.y + s.p2.y) / 2
        );

        Point t (
            s.p2.x - s.p1.x,
            s.p2.y - s.p1.y
        );

        double len = std::sqrt(t.x * t.x + t.y * t.y);

        t.x /= len;
        t.y /= len;

        // Перпендикуляр.
        Point n(-t.y, t.x);

        double sigma_parallel = 0.5 * len;
        double sigma_perp     = 0.15 * sigma_parallel;
        
        double xi_par  = std::normal_distribution(0.0, sigma_parallel)(gen);
        double xi_perp = std::normal_distribution(0.0, sigma_perp)(gen);

        // Перенос в глобальные координаты.
        Point q (
            mid.x + xi_par * t.x + xi_perp * n.x,
            mid.y + xi_par * t.y + xi_perp * n.y
        );
        q.is_rand = false;

        result.add(q);
    }

    if (opts.connect) {
        result = S.link_grid({}, result, S.stgs.nearest_count, opts.check_collision);
    }

    S.visual.picture({S.task, {.enhance = result}, "article2001_grid"});

    return result;
}
