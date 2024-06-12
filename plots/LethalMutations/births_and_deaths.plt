load "../init.plt"
set output "births_and_deaths.png"
set xlabel "year" 
set ylabel "count"
plot "lethal_2_statistics.txt" using (column("Year")) : (column("Births")) title "births - 2 lethal" smooth csplines with lines, \
     "lethal_2_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 2 lethal" smooth csplines with lines, \
     "lethal_3_statistics.txt" using (column("Year")) : (column("Births")) title "births - 3 lethal" smooth csplines with lines, \
     "lethal_3_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 3 lethal" smooth csplines with lines, \
     "lethal_4_statistics.txt" using (column("Year")) : (column("Births")) title "births - 4 lethal" smooth csplines with lines, \
     "lethal_4_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 4 lethal" smooth csplines with lines
load "../finalize.plt"
