set terminal push
set terminal png
set output "osobniki.png"
set xlabel "rok"
set ylabel "ilosc"
plot "output_averages_statistics.txt" using (column("Year")) : (column("Living_start")) title "na poczatku roku" with lines,"output_averages_statistics.txt" using (column("Year")) : (column("Living_end")) with lines title "na koniec roku"
set terminal pop
set output
replot
pause mouse close