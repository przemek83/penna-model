load "../init.plt"

set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
set logscale y
plot "lethal_2_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 2 lethal" with lp, \
     "lethal_3_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 3 lethal" with lp, \
     "lethal_4_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 4 lethal" with lp
load "../finalize.plt"