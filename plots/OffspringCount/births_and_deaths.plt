load "../init.plt"
set output "births_and_deaths.png"
set xlabel "year" 
set ylabel "count"
plot "count_2_statistics.txt" using (column("Year")) : (column("Births")) title "births - 2 x offspring" smooth csplines with lines, \
     "count_2_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 2 x offspring" smooth csplines with lines, \
     "count_3_statistics.txt" using (column("Year")) : (column("Births")) title "births - 3 x offspring" smooth csplines with lines, \
     "count_3_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 3 x offspring" smooth csplines with lines, \
     "count_4_statistics.txt" using (column("Year")) : (column("Births")) title "births - 4 x offspring" smooth csplines with lines, \
     "count_4_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 4 x offspring" smooth csplines with lines
load "../finalize.plt"
