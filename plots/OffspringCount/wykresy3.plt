load "../init.plt"
set output "families.png"
set xlabel "year" 
set ylabel "count"
set logscale x
set logscale y
plot "count_2_families.txt" using (column("Year")) : (column("Families")) title "families - 2 x offspring" with lines, \
     "count_3_families.txt" using (column("Year")) : (column("Families")) title "families - 3 x offspring" with lines, \
     "count_4_families.txt" using (column("Year")) : (column("Families")) title "families - 4 x offspring" with lines
load "../finalize.plt"