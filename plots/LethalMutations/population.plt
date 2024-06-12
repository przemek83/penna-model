load "../init.plt"
set output "population.png"
set xlabel "year"
set ylabel "count"
plot "lethal_2_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 2 lethal" smooth csplines with lines, \
     "lethal_2_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 2 lethal" smooth csplines with lines, \
     "lethal_2_statistics.txt" using (column("Year")) : (column("Births")) title "births - 2 lethal" smooth csplines with lines, \
     "lethal_3_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 3 lethal" smooth csplines with lines, \
     "lethal_3_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 3 lethal" smooth csplines with lines, \
     "lethal_3_statistics.txt" using (column("Year")) : (column("Births")) title "births - 3 lethal" smooth csplines with lines, \
     "lethal_4_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 4 lethal" smooth csplines with lines, \
     "lethal_4_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 4 lethal" smooth csplines with lines, \
     "lethal_4_statistics.txt" using (column("Year")) : (column("Births")) title "births - 4 lethal" smooth csplines with lines
load "../finalize.plt"
