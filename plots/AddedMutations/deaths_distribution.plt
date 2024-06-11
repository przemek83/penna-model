load "../init.plt"
set output "deaths_distribution.png"
set xlabel "age" 
set ylabel "risk of death"
set yrange [0:1.2]
plot "added_1_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 1 added" with lines, \
     "added_2_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 2 added" with lines, \
     "added_3_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 3 added" with lines
load "../finalize.plt"
