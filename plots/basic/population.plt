load "../init.plt"
set output "population.png"
set xlabel "year"
set ylabel "count"
plot "averages_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start" smooth csplines with lines, \
     "averages_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end" smooth csplines with lines, \
     "averages_statistics.txt" using (column("Year")) : (column("Births")) title "births" smooth csplines with lines
load "../finalize.plt"