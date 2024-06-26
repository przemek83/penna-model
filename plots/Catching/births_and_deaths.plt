load "../init.plt"
set output "births_and_deaths.png"
set xlabel "year" 
set ylabel "count"
plot "10_percent_statistics.txt" using (column("Year")) : (column("Births")) title "births - 10% catched" smooth csplines with lines, \
     "10_percent_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 10% catched" smooth csplines with lines, \
     "15_percent_statistics.txt" using (column("Year")) : (column("Births")) title "births - 15% catched" smooth csplines with lines, \
     "15_percent_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 15% catched" smooth csplines with lines, \
     "20_percent_statistics.txt" using (column("Year")) : (column("Births")) title "births - 20% catched" smooth csplines with lines, \
     "20_percent_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 20% catched" smooth csplines with lines
load "../finalize.plt"
