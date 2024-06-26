load "../init.plt"
set output "families.png"
set xlabel "year" 
set ylabel "count"
set logscale x
set logscale y
plot "10_percent_families.txt" using (column("Year")) : (column("Families")) title "families - 10% catched" with lines, \
     "15_percent_families.txt" using (column("Year")) : (column("Families")) title "families - 15% catched" with lines, \
     "20_percent_families.txt" using (column("Year")) : (column("Families")) title "families - 20% catched" with lines
load "../finalize.plt"