load "../init.plt"
set output "families.png"
set xlabel "year" 
set ylabel "count"
set logscale x
set logscale y
plot "lethal_2_families.txt" using (column("Year")) : (column("Families")) title "families - 2 lethal" with lines, \
     "lethal_3_families.txt" using (column("Year")) : (column("Families")) title "families - 3 lethal" with lines, \
     "lethal_4_families.txt" using (column("Year")) : (column("Families")) title "families - 4 lethal" with lines
load "../finalize.plt"