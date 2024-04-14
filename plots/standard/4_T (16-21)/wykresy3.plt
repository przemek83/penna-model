set output "3_3_rodziny.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
set logscale y 
set logscale x 
plot \
"7/proces0_symulacja0_6.txt" title "rodziny: T = 4" with lines, \
"16/proces0_symulacja0_6.txt" title "rodziny: T = 3" with lines, \
"19/proces0_symulacja0_6.txt" title "rodziny: T = 2" with lines
set terminal png
replot
pause -1
