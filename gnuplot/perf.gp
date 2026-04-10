
if (!exists("filename") || !exists("output_dir") || !exists("data_dir")) {
    exit
}

set terminal pngcairo size 2000,2000 enhanced font "Arial,12"
set output output_dir.'/'.filename

set style data histograms
set style histogram rowstacked
set style fill solid 1.0 border -1
set boxwidth 0.8

data = data_dir.'/'.'perf.txt'

plot data using 2:xtic(1) title 'Total'
     # '' using 3 title 'Offline'
