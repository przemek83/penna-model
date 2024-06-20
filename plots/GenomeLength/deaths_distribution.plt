load "../init.plt"
set output "deaths_distribution.png"
set xlabel "age" 
set ylabel "risk of death"
set yrange [0:1.2]
set xrange [0:31]
plot "32_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 32 bit genome" with lines, \
     "64_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 64 bit genome" with lines, \
     "128_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 128 bit genome" with lines
load "../finalize.plt"
