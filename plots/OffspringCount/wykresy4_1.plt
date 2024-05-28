load "../init.plt"

set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
set logscale y
plot "count_2_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 2 x offspring" with lp, \
     "count_3_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 3 x offspring" with lp, \
     "count_4_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 4 x offspring" with lp
load "../finalize.plt"