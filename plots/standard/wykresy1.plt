set output "osobniki.png"
set xlabel "rok" 
set ylabel "ilosc"
plot "proces0_symulacja0_1.txt" using 1:2 title "na poczatku roku" with lines,"proces0_symulacja0_1.txt" using 1:4 with lines title "na koniec roku"
set terminal png
replot
replot
pause -1
