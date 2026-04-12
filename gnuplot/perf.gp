
if (!exists("filename") || !exists("output_dir") || !exists("data_dir")) {
    exit
}

set terminal pngcairo size 1000,1000 enhanced font "Arial,12"
set output output_dir.'/'.filename

data = data_dir.'/'.'perf.txt'

set datafile columnheaders
set datafile separator whitespace

stats data nooutput

set style data histograms
set style histogram rowstacked
set style fill solid 1.0 border -1
set boxwidth 0.8

set key autotitle columnhead
set key outside right

plot for [i=2:STATS_columns] data using i:xtic(1) title columnhead(i)
