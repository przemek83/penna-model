set output "gompertz.png"
set xlabel "rok" 
set ylabel "ilosc"
plot "proces0_symulacja0_5.txt" title "gompertz" with boxes
set terminal png
replot
pause -1
