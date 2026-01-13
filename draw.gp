
# Настройки.
load "data/area.gp"

# Входные файлы.
polygons       = 'data/polygons.txt'
# polylines = 'data/polylines.txt'
grids          = 'data/grid.txt'
examined_roads = 'data/examined_roads.txt'
invalid_roads  = 'data/invalid_roads.txt'
path           = 'data/path.txt'

# Размеры поля.
set xrange [x_min-5:x_max+5]
set yrange [y_min-5:y_max+5]

# Легенда.
set key outside

# Отрисовка границ карты.
array x1[4] = [x_min, x_min, x_min, x_max]
array y1[4] = [y_min, y_max, y_min, y_min]
array x2[4] = [x_max, x_max, x_min, x_max]
array y2[4] = [y_min, y_max, y_max, y_max]

do for [i=1:4] {
    set arrow i from x1[i], y1[i] to x2[i], y2[i] nohead lw 2 lc rgb "black"
}

# Отрисовка карты.
plot \
    polygons       using 1:2 with filledcurves fc "royalblue" notitle, \
    grids          using 1:2 with linespoints lw 2 pt 7 lc "purple" title "Grid", \
    examined_roads using 1:2 with lines       lw 2      lc "gold" title "Examined roads", \
    invalid_roads  using 1:2 with lines       lw 2      lc "red" title "Invalid roads", \
    path           using 1:2 with linespoints lw 2 pt 7 lc "green" title "Path", \
    # polylines using 1:2 with lines lw 2 lc "blue" notitle, \

pause -1
