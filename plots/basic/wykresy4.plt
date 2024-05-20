load "init.plt"
set output "age_distribution.png"
set xlabel "year" 
set ylabel "count"
plot "output_averages_age_distribution.txt" using (column("Bit")) : (column("Count")) title "age distribution" with boxes
load "finalize.plt"