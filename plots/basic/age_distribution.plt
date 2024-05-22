load "init.plt"
set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
plot "output_averages_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution" with boxes
load "finalize.plt"