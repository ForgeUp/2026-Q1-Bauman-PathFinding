#pragma once

#include <algorithm>

#include "types/Point.hpp"
#include "types/Segment.hpp"

#include "math/eq.hpp"


namespace geometry {

// Обрезает отрезок по границам области.
// https://en.wikipedia.org/wiki/Liang%E2%80%93Barsky_algorithm
auto clip(const Segment& s, const Point& min, const Point& max) {
    struct Result {
        Segment segment;
        bool is_success{false};
    };

    const auto& x1 = s.p1.x;
    const auto& x2 = s.p2.x;
    const auto& y1 = s.p1.y;
    const auto& y2 = s.p2.y;

    auto maxi = [](double arr[], int n) -> double {
        double m = 0;
        for (int i = 0; i < n; ++i)
            if (m < arr[i]) m = arr[i];
        return m;
    };

    auto mini = [](double arr[], int n) -> double {
        double m = 1;
        for (int i = 0; i < n; ++i)
            if (m > arr[i]) m = arr[i];
        return m;
    };

    double p1 = -(x2 - x1);
    double p2 = -p1;
    double p3 = -(y2 - y1);
    double p4 = -p3;

    double q1 = x1 - min.x;
    double q2 = max.x - x1;
    double q3 = y1 - min.y;
    double q4 = max.y - y1;

    double exitParams[5], entryParams[5];
    int exitIndex = 1, entryIndex = 1;
    exitParams [0] = 1;
    entryParams[0] = 0;

    // Параллельная одной из границ области линия, расположенная вне области.
    if (math::eq(p1,0) && q1 < 0 || math::eq(p2,0) && q2 < 0 || math::eq(p3,0) && q3 < 0 || math::eq(p4,0) && q4 < 0) return Result{.is_success = false};

    if (!math::eq(p1,0)) {
        double r1 = q1 / p1;
        double r2 = q2 / p2;
        entryParams[entryIndex++] = p1 < 0 ? r1 : r2;
        exitParams [exitIndex++]  = p1 < 0 ? r2 : r1;
    }
    if (!math::eq(p3,0)) {
        double r3 = q3 / p3;
        double r4 = q4 / p4;
        entryParams[entryIndex++] = p3 < 0 ? r3 : r4;
        exitParams [exitIndex++]  = p3 < 0 ? r4 : r3;
    }

    double u1, u2;
    u1 = maxi(entryParams, entryIndex);
    u2 = mini(exitParams, exitIndex);

    if (u1 > u2) return Result{.is_success = false};

    return Result{
        .segment = {
            Point(
                std::clamp(x1 + (x2 - x1) * u1, min.x, max.x),
                std::clamp(y1 + (y2 - y1) * u1, min.y, max.y)
            ),
            Point(
                std::clamp(x1 + (x2 - x1) * u2, min.x, max.x),
                std::clamp(y1 + (y2 - y1) * u2, min.y, max.y)
            )
        },
        .is_success = true
    };
}

}
