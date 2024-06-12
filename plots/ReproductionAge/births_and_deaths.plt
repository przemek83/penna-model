load "../init.plt"
set output "births_and_deaths.png"
set xlabel "year" 
set ylabel "count"
plot "age_8_statistics.txt" using (column("Year")) : (column("Births")) title "births - age 8" smooth csplines with lines, \
     "age_8_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - age 8" smooth csplines with lines, \
     "age_10_statistics.txt" using (column("Year")) : (column("Births")) title "births - age 10" smooth csplines with lines, \
     "age_10_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - age 10" smooth csplines with lines, \
     "age_12_statistics.txt" using (column("Year")) : (column("Births")) title "births - age 12" smooth csplines with lines, \
     "age_12_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - age 12" smooth csplines with lines
load "../finalize.plt"
