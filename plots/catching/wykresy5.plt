set output "rozk_bitow.png"
set xlabel "rok" 
set ylabel "ilosc"
set yrange [0:1.1]
plot "proces0_symulacja0_4.txt" title "rozklad bitow" with boxes
set terminal png
replot
pause -1
