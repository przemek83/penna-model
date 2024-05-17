set terminal push
set terminal png size 1024,768
set output "population.png"
set xlabel "year"
set ylabel "individuals"
plot "output_averages_statistics.txt" using (column("Year")) : (column("Living_start")) title "living on year start" with lines, \
     "output_averages_statistics.txt" using (column("Year")) : (column("Living_start") - column("Deaths")) with lines title "survived on year end"
set terminal pop
set output
replot
pause mouse close