load "../init.plt"
set output "families.png"
set xlabel "year" 
set ylabel "count"
set logscale x
set logscale y
plot "averages_families.txt" using (column("Year")) : (column("Families")) title "families" with lines
load "../finalize.plt"