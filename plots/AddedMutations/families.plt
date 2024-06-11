load "../init.plt"
set output "families.png"
set xlabel "year" 
set ylabel "count"
set logscale x
set logscale y
plot "added_1_families.txt" using (column("Year")) : (column("Families")) title "families - 1 added" with lines, \
     "added_2_families.txt" using (column("Year")) : (column("Families")) title "families - 2 added" with lines, \
     "added_3_families.txt" using (column("Year")) : (column("Families")) title "families - 3 added" with lines
load "../finalize.plt"