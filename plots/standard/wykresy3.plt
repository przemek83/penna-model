set output "rodziny.png"
set xlabel "rok" 
set ylabel "ilosc"
plot "proces0_symulacja0_6.txt" title "rodziny" with lines
set terminal png
replot
pause -1
