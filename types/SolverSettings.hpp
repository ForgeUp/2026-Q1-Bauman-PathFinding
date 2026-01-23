#pragma once


// Структура с настройками генерации сетки перемещений.
struct SolverSettings {
    double squares_per_side{10}; // Количество ячеек по каждой из осей.
    double attempts_count{5}; // Число попыток разместить точку внутри ячейки.
};
