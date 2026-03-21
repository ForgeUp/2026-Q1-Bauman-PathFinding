#pragma once

#include <algorithm>
#include <set>
#include <stack>
#include <vector>

#include "KDTree.hpp"

#include "types/Point.hpp"


KDTree::KDTree(const std::set<Point>& input) {
    std::vector<Point> ps(input.begin(), input.end());
    root = KDTree::make_tree(ps.begin(), ps.end(), 0);
}

KDNode* KDTree::make_tree(const std::vector<Point>::iterator& begin, const std::vector<Point>::iterator& end, int32_t level) {
    if (begin == end) return nullptr;

    const size_t dim = 2;
    const auto mid = begin + std::distance(begin, end) / 2;
    std::nth_element(begin, mid, end, [level](auto& l, auto& r) {
        return l[level] < r[level];
    });

    size_t next_level = (level + 1) % dim;
    KDNode* left  = make_tree(begin,          mid, next_level);
    KDNode* right = make_tree(std::next(mid), end, next_level);

    return new KDNode(*mid, left, right);
}

KDTree::~KDTree() {
    if (root == nullptr) return;

    std::stack<KDNode*, std::vector<KDNode*>> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        auto node = nodes.top();
        nodes.pop();

        if (node->right != nullptr) nodes.push(node->right);
        if (node->left  != nullptr) nodes.push(node->left);

        delete node;
    }
}
