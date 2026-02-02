#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <queue>

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"
#include "types/Task.hpp"
#include "types/Solution.hpp"

#include "geometry/intersect.hpp"

#include "./impl/heuristic.hpp"


namespace pathfind {

struct Result {
    Graph path;
    Graph examined;
    Graph invalid;
    bool is_unreachable{false};
};

using Node = Point;

Graph trace_path(const Node& start, const Node& end, std::map<Node, Node>& cameFrom) {
    Graph path;

    if (start == end) {
        path.add(start);
        return path;
    }

    if (!cameFrom.contains(end)) return path;
    Node prev{end}, curr{cameFrom[end]};

    while (curr != start) {
        path.add(prev, curr);

        if (!cameFrom.contains(end)) return path;
        prev = std::exchange(curr, cameFrom[curr]);
    }
    path.add(prev, curr);
    
    return path;
}

// Ищет эвристический самый короткий путь между двумя вершинами БЕЗ проверки коллизии вершин/ребёр с препятствиями.
Result lazy(const Task& task, Graph& graph) {
    std::priority_queue<
        std::pair<double, Node>,
        std::vector<std::pair<double, Node>>,
        std::greater<>
    > open;

    std::map<Node, double> gScore;
    std::map<Node, Node> cameFrom;

    Graph examined;

    gScore[task.start] = 0.0;
    open.push({impl::heuristic(task.start, task.end), task.start});

    while (!open.empty()) {
        Node current = open.top().second;
        open.pop();

        if (current == task.end) {
            auto path = trace_path(task.start, task.end, cameFrom);
            return {path,examined,{},false};
        }

        for (const Node& neighbor : graph.adj[current]) {
            Segment edge{current, neighbor};
            examined.add(edge);

            double tentative_g = gScore[current] + impl::heuristic(current, neighbor);

            if (!gScore.contains(neighbor) || tentative_g < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_g;

                double fScore = tentative_g + impl::heuristic(neighbor, task.end);

                open.push({fScore, neighbor});
            }
        }
    }

    return {{},examined,{},true};
}

}
