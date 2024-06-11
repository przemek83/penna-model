load "../init.plt"
set output "deaths_distribution.png"
set xlabel "age" 
set ylabel "risk of death"
set yrange [0:1.2]
plot "count_2_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 2 x offspring" with lines, \
     "count_3_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 3 x offspring" with lines, \
     "count_4_deaths_distribution.txt" using (column("Bit")) : (column("Percent")) title "mortality - 4 x offspring" with lines
load "../finalize.plt"
