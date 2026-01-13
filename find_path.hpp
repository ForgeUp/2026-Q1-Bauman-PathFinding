#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <queue>

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Task.hpp"
#include "types/Solution.hpp"

#include "geometry/intersect.hpp"


using result_t = std::tuple<std::vector<Point>,std::vector<Segment>,std::vector<Segment>,bool>;

double heuristic(const Point& a, const Point& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

bool is_edge_valid(const Segment& s, const Task& task) {
    for (const auto& rock : task.area.rocks) {
        if (geometry::intersect(s, rock)) {
            return false;
        }
    }
    return true;
}

result_t find_path(const Task& task, const std::vector<Segment>& roads) {
    using Node = Point;

    struct PointHash {
        std::size_t operator()(const Point& p) const {
            std::size_t hx = std::hash<double>{}(p.x);
            std::size_t hy = std::hash<double>{}(p.y);
            return hx ^ (hy << 1);
        }
    };

    std::unordered_map<Node, std::vector<Node>, PointHash> adj;
    for (const auto& s : roads) {
        adj[s.p1].push_back(s.p2);
        adj[s.p2].push_back(s.p1);
    }

    std::priority_queue<
        std::pair<double, Node>,
        std::vector<std::pair<double, Node>>,
        std::greater<>
    > open;

    std::unordered_map<Node, double, PointHash> gScore;
    std::unordered_map<Node, Node, PointHash> cameFrom;

    std::vector<Segment> invalid;
    std::set<Segment> invalid_cache;

    std::vector<Segment> examined;
    std::set<Segment> examined_cache;

    gScore[task.start] = 0.0;
    open.push({heuristic(task.start, task.end), task.start});

    while (!open.empty()) {
        Node current = open.top().second;
        open.pop();

        if (current == task.end) {
            std::vector<Point> path;
            for (Node p = current; cameFrom.count(p); p = cameFrom[p]) {
                path.push_back(p);
            }
            path.push_back(task.start);
            std::reverse(path.begin(), path.end());
            return {path,examined,invalid,false};
        }

        for (const Node& neighbor : adj[current]) {
            Segment edge{current, neighbor};

            if (invalid_cache.contains(edge)) continue;
            if (!is_edge_valid(edge, task)) {
                invalid_cache.insert(edge);
                invalid.push_back(edge);
                continue;
            }

            if (!examined_cache.contains(edge)) {
                examined_cache.insert(edge);
                examined.push_back(edge);
            }

            double tentative_g = gScore[current] + heuristic(current, neighbor);

            if (!gScore.contains(neighbor) || tentative_g < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_g;

                double fScore = tentative_g + heuristic(neighbor, task.end);

                open.push({fScore, neighbor});
            }
        }
    }

    return {{},examined,invalid,true};
}
