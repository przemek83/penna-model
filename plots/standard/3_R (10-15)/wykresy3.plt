set output "2_3_rodziny.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
set logscale y 
set logscale x 
plot \
"7/proces0_symulacja0_6.txt" title "rodziny: R = 10" with lines, \
"10/proces0_symulacja0_6.txt" title "rodziny: R = 8" with lines, \
"13/proces0_symulacja0_6.txt" title "rodziny: R = 12" with lines
set terminal png
replot
pause -1
