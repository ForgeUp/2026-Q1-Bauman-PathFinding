#pragma once

#include <cmath>
#include <limits>

#include "qtree.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"

#include "geometry/mid.hpp"
#include "geometry/dist.hpp"
#include "geometry/clip.hpp"
#include "geometry/line_intersection.hpp"

#include "math/eq.hpp"
#include "math/sign.hpp"


// Метод для растеризации квадрантов вдоль отрезка. Предусмотрена обрезка отрезков, выходящих за пределы области дерева.
void Qtree::rasterize(const Segment& s) {
    auto [clipped, is_success] = geometry::clip(s, min, max);
    if (!is_success) return; // Отрезок вне области дерева.

    const auto& p1 = clipped.p1;
    const auto& p2 = clipped.p2;

    const double step = min_box_size_point * 0.5;

    Box* b1 = locate(p1); if (b1 == nullptr) {
        std::cout << "Wtf whata fa" << '\n';
        return;
    }
    Box* b2 = locate(p2); if (b2 == nullptr) {
        std::cout << "Wtf whata fa" << '\n';
        return;
    }

    if (b1 != nullptr) {
        b1 = upscale(b1, p1);
        b1->type = b1->type == Type::Busy ? Type::Busy : Type::Mix; 
    }
    if (b2 != nullptr) {
        b2 = upscale(b2, p2);
        b2->type = b2->type == Type::Busy ? Type::Busy : Type::Mix;
    }
    
    if (b1 == b2) return; // Отрезок-точка или отрезок, вмещающийся в одну клетку.

    if (math::eq(p2.x - p1.x, 0)) { // Вертикальная линия.
        double dy = math::sign(p2.y - p1.y) * step;
        for (Point p = p1; !math::eq(p.y, p2.y, step); p.y += dy) {
            Box* b = locate(p);
            if (b == nullptr) continue;
            b = upscale(b, p);
            b->type = b->type == Type::Busy ? Type::Busy : Type::Mix;
        }
        return;
    }

    if (math::eq(p2.y - p1.y, 0)) { // Горизонтальная линия.
        double dx = math::sign(p2.x - p1.x) * step;
        for (Point p = p1; !math::eq(p.x, p2.x, step); p.x += dx) {
            Box* b = locate(p);
            if (b == nullptr) continue;
            b = upscale(b, p);
            b->type = b->type == Type::Busy ? Type::Busy : Type::Mix;
        }
        return;
    }

    auto y_limit = [&p1, &p2](Box* b) -> double {
        return math::ge(p2.y - p1.y, 0) ? b->y_max : b->y_min;
    };

    auto x_limit = [&p1, &p2](Box* b) -> double {
        return math::ge(p2.x - p1.x, 0) ? b->x_max : b->x_min;
    };

    auto horz_line = [&p1, &p2, &x_limit](Box* b) -> Segment {
        double x = x_limit(b);
        return Segment{Point(x, b->y_min), Point(x, b->y_max)};
    };

    auto vert_line = [&p1, &p2, &y_limit](Box* b) -> Segment {
        double y = y_limit(b);
        return Segment{Point(b->x_min, y), Point(b->x_max, y)};
    };

    double dx = math::sign(p2.x - p1.x) * 0.001;
    double dy = math::sign(p2.y - p1.y) * 0.001;

    Box* b = b1;
    while (b != b2) {
        auto [qh, _1] = geometry::line::intersection(s, horz_line(b));
        auto [qv, _2] = geometry::line::intersection(s, vert_line(b));
        
        double dist_h = geometry::dist({b->x_mid,b->y_mid}, qh);
        double dist_v = geometry::dist({b->x_mid,b->y_mid}, qv);

        Point q;
        if (math::eq(dist_h, dist_v)) {
            q.x = qh.x + dx; 
            q.y = qh.y + dy;
        } else if (dist_h < dist_v) {
            q.x = qh.x + dx; 
            q.y = qh.y + dy;
        } else if (dist_v < dist_h) {
            q.x = qv.x + dx; 
            q.y = qv.y + dy;
        }

        Box* bq = locate(q);
        if (bq == nullptr) break;
        bq = upscale(bq, q);
        bq->type = bq->type == Type::Busy ? Type::Busy : Type::Mix;

        b = bq;
    }
}
