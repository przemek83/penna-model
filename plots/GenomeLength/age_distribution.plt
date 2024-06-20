load "../init.plt"

set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
set logscale y
set xrange [0:31]
plot "32_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 32 bit genome" with lp, \
     "64_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 64 bit genome" with lp, \
     "128_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 128 bit genome" with lp
load "../finalize.plt"