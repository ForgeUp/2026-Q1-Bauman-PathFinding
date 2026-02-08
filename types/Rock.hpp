#pragma once

#include <vector>

#include "types/Point.hpp"
#include "types/Segment.hpp"


struct Rock {
    std::vector<Point> points;

public:
    class EdgeIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Segment;
        using difference_type   = std::ptrdiff_t;
        using reference         = value_type;
        using pointer           = void;

        EdgeIterator(const Rock& r_, std::size_t i) : r(r_), index(i) {}

        Segment operator*() const {
            std::size_t n = r.points.size();
            return Segment{
                r.points[index],
                r.points[(index + 1) % n]
            };
        }

        EdgeIterator& operator++() {
            ++index;
            return *this;
        }

        bool operator==(const EdgeIterator& other) const {
            return index == other.index && &r == &other.r;
        }

        bool operator!=(const EdgeIterator& other) const {
            return !(*this == other);
        }

    private:
        const Rock& r;
        std::size_t index;
    };

    class EdgeRange {
    public:
        explicit EdgeRange(const Rock& r_) : r(r_) {}

        EdgeIterator begin() const {
            return EdgeIterator(r, 0);
        }

        EdgeIterator end() const {
            return EdgeIterator(r, r.points.size());
        }

    private:
        const Rock& r;
    };

    EdgeRange edges() const {
        return EdgeRange(*this);
    }

public:
    friend std::ostream& operator<<(std::ostream& os, const Rock& r) {
        for (auto& p : r.points) {
            os << p.x << ' ' << p.y << '\n';
        }
        os << '\n';
        return os;
    }
};
