#pragma once

#include "Base.hpp"


/**
 * @brief 
 *      Проверяет вершины и рёбра пути на коллизию с препятствиями.
 *      Сначала проверяет вершины и удаляет невалидные вершины и инцидентные рёбра из маршрутной карты, затем - рёбра.
 * @return bool
 *      Возвращает значение флага repeat = true, если путь не валиден и необходимо найти другой путь, иначе возвращает repeat = false.
 */
template <typename Derived>
bool CollisionChecker::Base<Derived>::check_path_collision() {
    auto& S = self();
    
    bool repeat = false;

    repeat |= S.check_points_collision(); 
    repeat |= S.check_edges_collision(); 

    return repeat;
}
