load "../init.plt"
set output "population.png"
set xlabel "year"
set ylabel "count"
plot "count_2_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 2 x offspring" smooth csplines with lines, \
     "count_2_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 2 x offspring" smooth csplines with lines, \
     "count_2_statistics.txt" using (column("Year")) : (column("Births")) title "births - 2 x offspring" smooth csplines with lines, \
     "count_3_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 3 x offspring" smooth csplines with lines, \
     "count_3_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 3 x offspring" smooth csplines with lines, \
     "count_3_statistics.txt" using (column("Year")) : (column("Births")) title "births - 3 x offspring" smooth csplines with lines, \
     "count_4_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 4 x offspring" smooth csplines with lines, \
     "count_4_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 4 x offspring" smooth csplines with lines, \
     "count_4_statistics.txt" using (column("Year")) : (column("Births")) title "births - 4 x offspring" smooth csplines with lines
load "../finalize.plt"
