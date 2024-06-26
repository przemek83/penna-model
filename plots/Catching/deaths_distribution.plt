load "../init.plt"
set output "deaths_distribution.png"
set xlabel "age" 
set ylabel "risk of death"
set yrange [0:1.2]
plot "10_percent_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 10% catched" with lines, \
     "15_percent_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 15% catched" with lines, \
     "20_percent_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 20% catched" with lines
load "../finalize.plt"
