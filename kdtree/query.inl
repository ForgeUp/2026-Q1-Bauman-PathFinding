#pragma once

#include <algorithm>
#include <stack>
#include <vector>
#include <numeric>

#include "KDTree.hpp"

#include "types/Point.hpp"


std::vector<Point> KDTree::nearest_k(const Point& p, int32_t k) {
    if (root == nullptr || k < 1) return {};

    struct Dist2Point {
        double dist2;
        Point p;
    };

    std::vector<Dist2Point> points;
    points.reserve(k);

    auto cmp = [](const Dist2Point& a, const Dist2Point& b) { return a.dist2 < b.dist2; };

    auto insert = [&points, k, &cmp](const Point& q, double dist2) {
        if (points.size() < k) {
            points.push_back({dist2, q});
            if (points.size() == k) {
                std::make_heap(points.begin(), points.end(), cmp);
            }
        } else if (dist2 < points.front().dist2) {
            std::pop_heap(points.begin(), points.end(), cmp);
            points.back() = {dist2, q};
            std::push_heap(points.begin(), points.end(), cmp);
        }
    };

    auto get_furthest = [&points]() -> double {
        return points.empty() ? std::numeric_limits<double>::max() : points.front().dist2;
    };

    auto find_dist2 = [dim = this->dim](const Point& a, const Point& b) {
        double result = 0;
        for (int32_t i = 0; i < dim; ++i) {
            result += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return result;
    };

    struct NodeLevel {
        KDNode* node;
        int32_t level;
    };

    std::stack<NodeLevel, std::vector<NodeLevel>> stack;
    stack.push({root, 0});

    while (!stack.empty()) {
        auto [node, level] = stack.top();
        const auto& q = node->p;
        stack.pop();

        if (node == nullptr) continue;

        double dist2 = find_dist2(p, q);
        insert(q, dist2);

        double delta = p[level] - q[level];
        double delta2 = delta * delta;

        KDNode* near_child = delta < 0 ? node->left  : node->right;
        KDNode* far_child  = delta < 0 ? node->right : node->left;

        if (near_child != nullptr) stack.push({near_child, (level + 1) % dim});

        if (far_child != nullptr && (points.size() < k || delta2 < get_furthest())) {
            stack.push({far_child, (level + 1) % dim});
        }
    }

    std::sort(points.begin(), points.end(), [](const Dist2Point& a, const Dist2Point& b) {
        return a.dist2 < b.dist2;
    });

    std::vector<Point> result;
    result.reserve(points.size());
    for (const auto& dp : points) result.push_back(dp.p);

    return result;
}
