#pragma once

#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <ostream>

#include "types/Point.hpp"
#include "types/Segment.hpp"


class Graph {
public:
    // Список вершин.
    std::set<Point> verts;

    // Матрица смежности.
    std::map<Point,std::set<Point>> adj;

    // Количество рёбер.
    int32_t edges_count{0};

public:
// Вершины.
    void add(const Point& p) {
        if (verts.contains(p)) return;
        verts.insert(p);
        adj[p] = {};
    }

    void remove(const Point& p) {
        if (!verts.contains(p)) return;
        verts.erase(p);
        for (auto& q : adj[p]) {
            adj[q].erase(p);
            edges_count--;
        }
        adj.erase(p);
    }

    bool contains(const Point& p) {
        return verts.contains(p);
    }

// Рёбра.
    void add(const Point& p1, const Point& p2) {
        if (!verts.contains(p1)) {
            verts.insert(p1);
            adj[p1] = {};
        }
        if (!verts.contains(p2)) {
            verts.insert(p2);
            adj[p2] = {};
        }
        if (adj[p1].contains(p2)) return;
        adj[p1].insert(p2);
        adj[p2].insert(p1);
        edges_count++;
    }

    void add(const Segment& s) {
        add(s.p1, s.p2);
    }

    void remove(const Point& p1, const Point& p2) {
        if (!verts.contains(p1) || !verts.contains(p2)) return;
        if (!adj[p1].contains(p2)) return;
        adj[p1].erase(p2);
        adj[p2].erase(p1);
        edges_count--;
    }

    void remove(const Segment& s) {
        remove(s.p1, s.p2);
    }

    bool contains(const Point& p1, const Point& p2) {
        if (!verts.contains(p1) || !verts.contains(p2)) return false;
        return adj[p1].contains(p2);
    }

    bool contains(const Segment& s) {
        return contains(s.p1, s.p2);
    }

// Межграфовое взаимодействие.
    void join(const Graph& other) {
        auto& ths = *this;
        for (const auto& p : other.verts) {
            ths.add(p);
        }
        for (const auto& [p, neighbours] : other.adj) {
            for (const auto& q : neighbours) {
                if (ths.adj[p].contains(q)) continue;
                ths.adj[p].insert(q);
                ths.adj[q].insert(p);
                edges_count++;
            }
        }
    }

public:
    friend std::ostream& operator<<(std::ostream& os, const Graph& g_) {
        Graph& g = const_cast<Graph&>(g_); // [TODO] Исправить этот костыль.
        for (const auto& p : g.verts) {
            for (const auto& q : g.adj[p]) {
                if (p < q) continue;
                os << Segment(p,q) << '\n';
            }
        }
        return os;
    }

};
