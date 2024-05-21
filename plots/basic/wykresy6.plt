load "init.plt"
set output "deaths_distribution.png"
set xlabel "year" 
set ylabel "risk of death"
set yrange [0:1.1]
plot "output_averages_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality" with lines
load "finalize.plt"
