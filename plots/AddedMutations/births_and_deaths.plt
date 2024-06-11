load "../init.plt"
set output "births_and_deaths.png"
set xlabel "year" 
set ylabel "count"
plot "added_1_statistics.txt" using (column("Year")) : (column("Births")) title "births - 1 added" smooth csplines with lines, \
     "added_1_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 1 added" smooth csplines with lines, \
     "added_2_statistics.txt" using (column("Year")) : (column("Births")) title "births - 2 added" smooth csplines with lines, \
     "added_2_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 2 added" smooth csplines with lines, \
     "added_3_statistics.txt" using (column("Year")) : (column("Births")) title "births - 3 added" smooth csplines with lines, \
     "added_3_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 3 added" smooth csplines with lines
load "../finalize.plt"
