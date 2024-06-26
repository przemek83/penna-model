load "../init.plt"
set output "population.png"
set xlabel "year"
set ylabel "count"
plot "10_percent_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 10% catched" smooth csplines with lines, \
     "10_percent_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 10% catched" smooth csplines with lines, \
     "10_percent_statistics.txt" using (column("Year")) : (column("Births")) title "births - 10% catched" smooth csplines with lines, \
     "15_percent_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 15% catched" smooth csplines with lines, \
     "15_percent_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 15% catched" smooth csplines with lines, \
     "15_percent_statistics.txt" using (column("Year")) : (column("Births")) title "births - 15% catched" smooth csplines with lines, \
     "20_percent_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 20% catched" smooth csplines with lines, \
     "20_percent_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 20% catched" smooth csplines with lines, \
     "20_percent_statistics.txt" using (column("Year")) : (column("Births")) title "births - 20% catched" smooth csplines with lines
load "../finalize.plt"
