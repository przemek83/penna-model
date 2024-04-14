set output "zycia.png"
set xlabel "rok" 
set ylabel "ilosc"
plot "proces0_symulacja0_1.txt" using 1:3 title "przyrost" with lines smooth bezier,"proces0_symulacja0_1.txt" using 1:5 with lines title "zgony" smooth bezier
set terminal png
replot
pause -1
