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

#include "./impl/heuristic.hpp"


namespace pathfind {

using result_t = std::tuple<std::vector<Point>,Graph,Graph,bool>;

result_t with_collision_check(const Task& task, Graph& graph) {
    using Node = Point;

    std::priority_queue<
        std::pair<double, Node>,
        std::vector<std::pair<double, Node>>,
        std::greater<>
    > open;

    std::map<Node, double> gScore;
    std::map<Node, Node> cameFrom;

    Graph invalid;
    Graph examined;

    gScore[task.start] = 0.0;
    open.push({impl::heuristic(task.start, task.end), task.start});

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

        for (const Node& neighbor : graph.adj[current]) {
            Segment edge{current, neighbor};
            if (!examined.contains(edge)) {
                examined.add(edge);
            }

            if (invalid.contains(edge)) continue;
            if (geometry::intersect(edge, task.area)) {
                invalid.add(edge);
                continue;
            }

            double tentative_g = gScore[current] + impl::heuristic(current, neighbor);

            if (!gScore.contains(neighbor) || tentative_g < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_g;

                double fScore = tentative_g + impl::heuristic(neighbor, task.end);

                open.push({fScore, neighbor});
            }
        }
    }

    return {{},examined,invalid,true};
}

}
