load "../init.plt"
set output "deaths_distribution.png"
set xlabel "age" 
set ylabel "risk of death"
set yrange [0:1.2]
plot "age_8_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - age 8" with lines, \
     "age_10_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - age 10" with lines, \
     "age_12_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - age 12" with lines
load "../finalize.plt"
