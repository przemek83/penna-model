load "../init.plt"
set output "births_and_deaths.png"
set xlabel "year" 
set ylabel "count"
plot "32_statistics.txt" using (column("Year")) : (column("Births")) title "births - 32 bit genome" smooth csplines with lines, \
     "32_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 32 bit genome" smooth csplines with lines, \
     "64_statistics.txt" using (column("Year")) : (column("Births")) title "births - 64 bit genome" smooth csplines with lines, \
     "64_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 64 bit genome" smooth csplines with lines, \
     "128_statistics.txt" using (column("Year")) : (column("Births")) title "births - 128 bit genome" smooth csplines with lines, \
     "128_statistics.txt" using (column("Year")) : (column("Deaths")) title "deaths - 128 bit genome" smooth csplines with lines
load "../finalize.plt"
