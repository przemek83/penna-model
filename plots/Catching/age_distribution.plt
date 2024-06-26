load "../init.plt"

set output "age_distribution.png"
set xlabel "age" 
set ylabel "count"
set logscale y
plot "10_percent_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 10% catched" with lp, \
     "15_percent_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 15% catched" with lp, \
     "20_percent_age_distribution.txt" using (column("Age")) : (column("Count")) title "age distribution - 20% catched" with lp
load "../finalize.plt"