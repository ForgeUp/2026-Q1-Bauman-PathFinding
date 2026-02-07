#pragma once

#include <stack>

#include "qtree.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"

#include "geometry/dist.hpp"
#include "geometry/clip.hpp"

#include "math/eq.hpp"


void Qtree::add(const Segment& s) {
    Point p1 = s.p1;
    Point p2 = s.p2;

    if (!root->is_inside(p1) || !root->is_inside(p2)) { // Обрезаем отрезок по границам области.
        auto [clipped, is_success] = geometry::clip(s, {root->x_min,root->y_min}, {root->x_max,root->y_max});
        if (!is_success) return; // Отрезок расположен вне области.
        p1 = clipped.p1;
        p2 = clipped.p2;
    }

    // Определяем квадраты, в которые входят точки.
    Box* b1 = locate(p1);
    Box* b2 = locate(p2);

    if (b1 == nullptr || b2 == nullptr) return; // Такого произойти не должно.

    // Квадраты совпадают -> квадрат НЕ минимального размера -> разбиваем квадрат, пока точки не будут в разных квадратах или квадрат не достигнет минимального размера.
    while (b1 == b2 && math::ge(b1->length(), min_box_size_point)) {
        Box* b = b1;
        expand(b);
        b1 = locate(p1,b);
        b2 = locate(p2,b);
    }

    // Квадраты совпадают -> квадрат уже минимального размера -> отрезок ещё меньше -> отрезок целиком вносится в квадрат.
    if (b1 == b2 && math::le(b1->length(), min_box_size_point)) {
        b1->type = b1->type == Type::Busy ? Type::Busy : Type::Mix;
        b1->segments.push_back(&s);
        return;
    }
    // Иначе точки в разных квадратах.

    // Уменьшаем изначальные квадраты до приемлемого размера для содержания точки.
    b1 = upscale(b1, p1);
    b2 = upscale(b2, p2);
    
    // Фиксируем состояние квадратов.
    b1->type = b1->type == Type::Busy ? Type::Busy : Type::Mix;
    b2->type = b2->type == Type::Busy ? Type::Busy : Type::Mix;
    
    b1->segments.push_back(&s);
    b2->segments.push_back(&s);

    // Формирование непрерывного следа от линии.
    using Pivot = std::pair<Box*,Point>;
    std::stack<std::pair<Pivot,Pivot>> check;
    
    if (!is_nearby(b1,b2)) check.emplace(Pivot{b1,p1}, Pivot{b2,p2});

    while (check.size() > 0) {
        auto [v1, v2] = check.top();
        auto [bl, pl] = v1;
        auto [br, pr] = v2;
        check.pop();

        // Вырожденный случай.
        bool is_deadend = false;

        // Промежуточная точка.
        Point pm = Point(
            (pl.x + pr.x) / 2,
            (pl.y + pr.y) / 2
        );
        Box* bm = locate(pm); // Определяем квадрат, в который входит промежуточная точка.

        // Поиск промежуточного квадрата.
        while (bm == bl || bm == br) {
            // Уменьшаем границы и ищем новую промежуточную точку.
            (bm == bl ? pl : pr) = pm;
            (bm == bl ? bl : br) = bm;

            pm = Point(
                (pl.x + pr.x) / 2,
                (pl.y + pr.y) / 2
            );

            bm = locate(pm);

            // Защита от бесконечной рекурсии при вырожденном случае.
            if (math::le(geometry::dist(pl,pm), min_box_size_point * 0.5)) {
                is_deadend = true;
                break;
            }
        }

        if (is_deadend) continue; // При вырожденном случае прекращаем поиск между данными квадратами.

        // Уменьшем квадрат до приемлемого размера для содержания точки.
        bm = upscale(bm, pm);

        // Фиксируем состояние промежуточного квадрата.
        bm->type = bm->type == Type::Busy ? Type::Busy : Type::Mix;
        bm->segments.push_back(&s);

        // Уточняем след для изначальных квадратов, с которыми НЕ соседствует промежуточный квадрат.
        if (!is_nearby(bm,br)) check.emplace(Pivot{bm,pm}, Pivot{br,pr});
        if (!is_nearby(bl,bm)) check.emplace(Pivot{bl,pl}, Pivot{bm,pm});
    }
}
