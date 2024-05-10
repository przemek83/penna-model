set terminal push
set terminal png
set output "osobniki.png"
set xlabel "rok" 
set ylabel "ilosc"
plot "output_averages_statistics.txt" using 1:2 title "na poczatku roku" with lines,"output_averages_statistics.txt" using 1:4 with lines title "na koniec roku"
set terminal pop
set output
replot
pause mouse close