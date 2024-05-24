load "init.plt"
set output "bits_distribution.png"
set xlabel "bit" 
set ylabel "percent of 1s"
set yrange [0:1.1]
set style fill solid border -1
plot "output_averages_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution" with boxes
load "finalize.plt"