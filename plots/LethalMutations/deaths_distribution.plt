load "../init.plt"
set output "deaths_distribution.png"
set xlabel "age" 
set ylabel "risk of death"
set yrange [0:1.2]
plot "lethal_2_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 2 lethal" with lines, \
     "lethal_3_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 3 lethal" with lines, \
     "lethal_4_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 4 lethal" with lines
load "../finalize.plt"
