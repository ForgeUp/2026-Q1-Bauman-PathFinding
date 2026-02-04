
# Настройки.
load "data/area.gp"

# Особы пунктир
set dashtype 8 (2,10) 

# Место для вывода.
if (exists("filename")) {
    set terminal pngcairo size 2000,1500 enhanced font "Arial,12"
    set output filename
}

# Входные файлы.
polygons    = 'data/polygons.txt'
grids       = 'data/grid.txt'
enhance     = 'data/enhance.txt'
examined    = 'data/examined.txt'
invalid     = 'data/invalid.txt'
invalid_all = 'data/invalid_all.txt'
path        = 'data/path.txt'

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
    grids          using 1:2 with linespoints lw 1 ps 0.5 pt 7 lc "purple"     title "Grid", \
    polygons       using 1:2 with filledcurves                 fc "royalblue"  notitle, \
    examined       using 1:2 with lines       lw 2             lc "gold"       title "Examined roads", \
    enhance        using 1:2 with points      lw 2 dt 8   pt 7 lc "dark-pink"  title "Enhancement", \
    path           using 1:2 with linespoints lw 2        pt 7 lc "green"      title "Path", \
    invalid        using 1:2 with lines       lw 2 dt 8        lc "red"        title "Invalid roads", \
    invalid_all    using 1:2 with lines       lw 2             lc "red"        title "All invalid roads", \
    '+' using (x_start):(y_start) with points      ps 2   pt 7 lc "green"      title "Start point", \
    '+' using (x_end)  :(y_end)   with points      ps 2   pt 7 lc "dark-green" title "End point", \

if (!exists("filename")) {
    pause -1
}
