#pragma once

#include "types/Point.hpp"


struct Segment {
    Point p1{};
    Point p2{};

    bool is_vert_rand() const {
        return p1.is_rand && p2.is_rand;
    }

    friend std::ostream& operator<<(std::ostream& os, const Segment& s) {
        os << s.p1 << s.p2 << '\n';
        return os;
    }

    friend bool operator<(const Segment& lhs, const Segment& rhs) {
        Point l1 = lhs.p1 < lhs.p2 ? lhs.p1 : lhs.p2;
        Point l2 = lhs.p1 < lhs.p2 ? lhs.p2 : lhs.p1;

        Point r1 = rhs.p1 < rhs.p2 ? rhs.p1 : rhs.p2;
        Point r2 = rhs.p1 < rhs.p2 ? rhs.p2 : rhs.p1;
        
        if (l1 < r1) return true;
        if (r1 < l1) return false;
        return l2 < r2;
    }

};
