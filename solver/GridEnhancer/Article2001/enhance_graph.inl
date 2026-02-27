#pragma once

#include <algorithm>
#include <numeric>
#include <random>

#include "Article2001.hpp"

#include "random/from_range.hpp"
#include "random/from_norm.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"


template <typename Derived>
bool GridEnhancer::Article2001<Derived>::enhance_graph() {
    auto& S = self();

    if (!S.is_path_not_found) return false;
    
    S.metric.time_in(__func__);
    
    static thread_local std::mt19937 rng{std::random_device{}()};

    // Генерация случайных точек в пределах всей арены.
    Graph enhance_rand_points = gridgen::lazy_points(S.stgs.enhance_rand_nodes_count, S.corner_min, S.corner_max);
    S.grid.join(enhance_rand_points);
    S.enhance.join(enhance_rand_points);

    // Генерация случайных точек вокруг отброшенных рёбер.

    std::vector<int32_t> idxs(S.invalid_all_rand.edges_count);
    std::iota(idxs.begin(), idxs.end(), 0);
    if (S.invalid_all_rand.edges_count >= S.stgs.enhance_seed_nodes_count) {
        std::shuffle(idxs.begin(), idxs.end(), rng);
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
        
        double xi_par  = random::from_norm(0.0, sigma_parallel, rng);
        double xi_perp = random::from_norm(0.0, sigma_perp, rng);

        // Перенос в глобальные координаты.
        Point q (
            mid.x + xi_par * t.x + xi_perp * n.x,
            mid.y + xi_par * t.y + xi_perp * n.y
        );
        q.is_rand = false;

        S.grid.add(q);
        S.enhance.add(q);
    }

    // Соединение рёбрами новых точек с уже имеющимися.
    S.grid = gridgen::lazy_roads(S.grid, S.stgs.connection_radius);
    
    // Удаление рёбер, коллизия с которыми уже была установлена.
    for (const auto& s : S.invalid_all.edges()) {
        S.grid.remove(s);
    }

    // Запись дополнительных рёбер.
    // for (const auto& p : enhance.verts) {
    //     for (const auto& q : grid.adj[p]) {
    //         enhance.add(p, q);
    //     }
    // }

    // S.visual.picture({S.task, S.sln, "point_enhancement"});

    S.metric.time_out(__func__);

    S.attempts++;
    return true;
}
