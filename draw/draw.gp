
if (!exists("filename") || !exists("output_dir") || !exists("data_dir")) {
    exit
}

set terminal pngcairo size 2000,1500 enhanced font "Arial,12"
set output output_dir.'/'.filename

# Настройки.
load data_dir.'/'.'area.gp'

# Особый пунктир
set dashtype 8 (2,10) 

# Входные файлы.
polygons     = data_dir.'/'.'polygons.txt'
grids        = data_dir.'/'.'grid.txt'
enhance      = data_dir.'/'.'enhance.txt'
examined     = data_dir.'/'.'examined.txt'
invalid      = data_dir.'/'.'invalid.txt'
invalid_all  = data_dir.'/'.'invalid_all.txt'
path         = data_dir.'/'.'path.txt'
qtree        = data_dir.'/'.'qtree.txt'
qtree_free   = data_dir.'/'.'qtree_free.txt'
qtree_mix    = data_dir.'/'.'qtree_mix.txt'
qtree_busy   = data_dir.'/'.'qtree_busy.txt'

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
    grids          using 1:2   with linespoints lw 1 ps 0.5 pt 7 lc "purple"     title "Grid", \
    polygons       using 1:2   with filledcurves                 fc "royalblue"  notitle, \
    examined       using 1:2   with lines       lw 2             lc "gold"       title "Examined roads", \
    enhance        using 1:2   with linespoints lw 2        pt 7 lc "dark-pink"  title "Enhancement", \
    path           using 1:2   with linespoints lw 2        pt 7 lc "green"      title "Path", \
    invalid        using 1:2   with linespoints lw 2             lc "red"        title "Invalid roads", \
    invalid_all    using 1:2   with linespoints lw 2             lc "red"        title "All invalid roads", \
    qtree_free     using 1:2   with filledcurves fs transparent solid 0.8 lc rgb "green"  notitle, \
    qtree_mix      using 1:2   with filledcurves fs transparent solid 0.8 lc rgb "red"    notitle, \
    qtree_busy     using 1:2   with filledcurves fs transparent solid 0.8 lc rgb "yellow" notitle, \
    qtree          using 1:2   with lines       lw 1             lc "grey"       title "Qtree Split", \
    '+' using (x_start):(y_start) with points      ps 2   pt 7 lc "green"      title "Start point", \
    '+' using (x_end)  :(y_end)   with points      ps 2   pt 7 lc "dark-green" title "End point", \

if (!exists("filename")) {
    pause -1
}
