load "../init.plt"
set output "population.png"
set xlabel "year"
set ylabel "count"
plot "added_1_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 1 added" smooth csplines with lines, \
     "added_1_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 1 added" smooth csplines with lines, \
     "added_1_statistics.txt" using (column("Year")) : (column("Births")) title "births - 1 added" smooth csplines with lines, \
     "added_2_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 2 added" smooth csplines with lines, \
     "added_2_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 2 added" smooth csplines with lines, \
     "added_2_statistics.txt" using (column("Year")) : (column("Births")) title "births - 2 added" smooth csplines with lines, \
     "added_3_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 3 added" smooth csplines with lines, \
     "added_3_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 3 added" smooth csplines with lines, \
     "added_3_statistics.txt" using (column("Year")) : (column("Births")) title "births - 3 added" smooth csplines with lines
load "../finalize.plt"
