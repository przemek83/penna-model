set output "rozk_wieku.png"
set xlabel "rok" 
set ylabel "ilosc"
plot "proces0_symulacja0_3.txt" title "rozklad wieku" with boxes
set terminal png
replot
pause -1
