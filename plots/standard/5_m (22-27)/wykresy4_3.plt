set output "4_4_3_rozk_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 500
set logscale y 
#set style fill solid border -1
plot [0:27] \
"9/proces0_symulacja0_3.txt" title "rozklad wieku: m = 1" with  lp, \
"24/proces0_symulacja0_3.txt" title "rozklad wieku: m = 2" with  lp,\
"27/proces0_symulacja0_3.txt" title "rozklad wieku: m = 3" with  lp
set terminal png
replot
pause -1