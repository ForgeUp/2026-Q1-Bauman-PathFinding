#pragma once

#include <cmath>

#include "types/Point.hpp"
#include "types/Segment.hpp"


namespace geometry {

// Выполняет параллельный перенос отрезка вдоль нормали.
Segment shift(const Segment& s, double delta) {
    using Vector = Point;
    Vector t ( // Касательный вектор.
        s.p2.x - s.p1.x,
        s.p2.y - s.p1.y
    ); 
    Vector n ( // Вектор нормали.
        +t.y,
        -t.x
    );
    double n_len = std::hypot(n.x, n.y); // Нормализация.
    n.x /= n_len; n.y /= n_len;

    return {
        Point(
            s.p1.x + n.x * delta,
            s.p1.y + n.y * delta
        ),
        Point(
            s.p2.x + n.x * delta,
            s.p2.y + n.y * delta
        )
    }; // Смещение ребра вдоль вектора нормали.
}

}
