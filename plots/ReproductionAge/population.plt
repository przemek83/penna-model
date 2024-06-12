load "../init.plt"
set output "population.png"
set xlabel "year"
set ylabel "count"
plot "age_8_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - age 8" smooth csplines with lines, \
     "age_8_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - age 8" smooth csplines with lines, \
     "age_8_statistics.txt" using (column("Year")) : (column("Births")) title "births - age 8" smooth csplines with lines, \
     "age_10_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - age 10" smooth csplines with lines, \
     "age_10_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - age 10" smooth csplines with lines, \
     "age_10_statistics.txt" using (column("Year")) : (column("Births")) title "births - age 10" smooth csplines with lines, \
     "age_12_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - age 12" smooth csplines with lines, \
     "age_12_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - age 12" smooth csplines with lines, \
     "age_12_statistics.txt" using (column("Year")) : (column("Births")) title "births - age 12" smooth csplines with lines
load "../finalize.plt"
