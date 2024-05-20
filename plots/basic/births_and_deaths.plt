load "init.plt"
set output "births_and_deaths.png"
set xlabel "year" 
set ylabel "count"
plot "output_averages_statistics.txt" using (column("Year")) : (column("Births")) title "births" smooth csplines with lines, \
     "output_averages_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths" smooth csplines with lines
load "finalize.plt"
