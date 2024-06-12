load "../init.plt"
set output "families.png"
set xlabel "year" 
set ylabel "count"
set logscale x
set logscale y
plot "age_8_families.txt" using (column("Year")) : (column("Families")) title "families - age 8" with lines, \
     "age_10_families.txt" using (column("Year")) : (column("Families")) title "families - age 10" with lines, \
     "age_12_families.txt" using (column("Year")) : (column("Families")) title "families - age 12" with lines
load "../finalize.plt"