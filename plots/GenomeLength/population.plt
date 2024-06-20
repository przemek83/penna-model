load "../init.plt"
set output "population.png"
set xlabel "year"
set ylabel "count"
plot "32_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 32 bit genome" smooth csplines with lines, \
     "32_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 32 bit genome" smooth csplines with lines, \
     "32_statistics.txt" using (column("Year")) : (column("Births")) title "births - 32 bit genome" smooth csplines with lines, \
     "64_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 64 bit genome" smooth csplines with lines, \
     "64_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 64 bit genome" smooth csplines with lines, \
     "64_statistics.txt" using (column("Year")) : (column("Births")) title "births - 64 bit genome" smooth csplines with lines, \
     "128_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start - 128 bit genome" smooth csplines with lines, \
     "128_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) title "survived on year end - 128 bit genome" smooth csplines with lines, \
     "128_statistics.txt" using (column("Year")) : (column("Births")) title "births - 128 bit genome" smooth csplines with lines
load "../finalize.plt"
