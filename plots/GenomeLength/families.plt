load "../init.plt"
set output "families.png"
set xlabel "year" 
set ylabel "count"
set logscale x
set logscale y
plot "32_families.txt" using (column("Year")) : (column("Families")) title "families - 32 bit genome" with lines, \
     "64_families.txt" using (column("Year")) : (column("Families")) title "families - 64 bit genome" with lines, \
     "128_families.txt" using (column("Year")) : (column("Families")) title "families - 128 bit genome" with lines
load "../finalize.plt"