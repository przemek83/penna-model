load "../init.plt"
set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
set logscale y
set style fill solid border -1
plot "averages_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution" with boxes
load "../finalize.plt"