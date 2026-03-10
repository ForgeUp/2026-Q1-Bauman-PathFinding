#pragma once

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include "Article2001.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация случайных точек вокруг отброшенных рёбер.
template <typename Derived>
Graph GridGenerator::Article2001<Derived>::generate_grid(Options& opts) {
    auto& S = self();
    
    S.metric.time_in(__func__);

    S.visual.picture({S.task, {.invalid = S.invalid}, "invalid"});
    S.invalid.clear();
    
    std::random_device rd;
    std::mt19937 gen(rd());

    Graph result;

    std::vector<int32_t> idxs(S.invalid_all_rand.edges_count);
    std::iota(idxs.begin(), idxs.end(), 0);
    if (S.invalid_all_rand.edges_count >= S.stgs.enhance_seed_nodes_count) {
        std::shuffle(idxs.begin(), idxs.end(), gen);
        idxs.resize(S.stgs.enhance_seed_nodes_count);
        std::sort(idxs.begin(), idxs.end());
    }
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

    if (opts.connect && opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count);
    } else if (opts.connect && !opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count); // [TODO] Заменить на вариант с проверкой коллизии.
    }

    S.visual.picture({S.task, {.enhance = result}, "article2001_grid"});

    S.metric.time_out(__func__);

    S.attempts++;
    return result;
}
