load "../init.plt"

set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
set logscale y
plot "added_1_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 1 added" with lp, \
     "added_2_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 2 added" with lp, \
     "added_3_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 3 added" with lp
load "../finalize.plt"