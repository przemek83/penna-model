set output "4_3_rodziny.png"
set xlabel "rok" 
set ylabel "ilosc"
set logscale y 
set logscale x 
set term png size 800, 500
plot  \
"7/proces0_symulacja0_6.txt" title "rodziny: m = 1" with lines, \
"22/proces0_symulacja0_6.txt" title "rodziny: m = 2" with lines, \
"25/proces0_symulacja0_6.txt" title "rodziny: m = 3" with lines
set terminal png
replot
pause -1
