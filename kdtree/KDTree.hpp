#pragma once

#include <set>
#include <vector>

#include "types/Point.hpp"


struct KDNode {
    Point p;
    KDNode* left{nullptr};
    KDNode* right{nullptr};

    KDNode() = default;
    KDNode(const Point& p_, KDNode* left_, KDNode* right_)  : p(p_), left(left_), right(right_) {}

    ~KDNode() = default;

    bool is_leaf() { return left == nullptr && right == nullptr; }
};

class KDTree {
private:
    KDNode* root;
    const int32_t dim{2};

public:
    KDTree() = default;
    explicit KDTree(const std::set<Point>& points);

    ~KDTree();

    KDTree(const KDTree&) = delete;
    KDTree& operator=(const KDTree&) = delete;
    KDTree(const KDTree&&) = delete;
    KDTree& operator=(const KDTree&&) = delete;

public:
    std::vector<Point> nearest_k(const Point& p, int32_t k);

private:
    KDNode* make_tree(const std::vector<Point>::iterator& begin, const std::vector<Point>::iterator& end, int32_t level);

};


#include "build.inl"
#include "query.inl"
