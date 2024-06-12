load "../init.plt"

set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
set logscale y
plot "age_8_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - age 8" with lp, \
     "age_10_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - age 10" with lp, \
     "age_12_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - age 12" with lp
load "../finalize.plt"