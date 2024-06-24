load "../init.plt"
set output "deaths_distribution.png"
set xlabel "age" 
set ylabel "risk of death"
set yrange [0:1.1]
set style fill solid border -1
plot "averages_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality" with boxes
load "../finalize.plt"
