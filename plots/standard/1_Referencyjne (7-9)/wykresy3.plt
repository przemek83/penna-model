set output "0_3_rodziny.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
set logscale y 
set logscale x 
plot \
"7/proces0_symulacja0_6.txt" title "rodziny: genom 32 bitowy" with lines, \
"8/proces0_symulacja0_6.txt" title "rodziny: genom 64 bitowy" with lines, \
"9/proces0_symulacja0_6.txt" title "rodziny: genom 128 bitowy" with lines
set terminal png
replot
pause -1
