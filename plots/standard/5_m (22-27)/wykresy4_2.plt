set output "4_4_2_rozk_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set logscale y 
set term png size 800, 500
#set style fill solid border -1
plot [0:25] \
"8/proces0_symulacja0_3.txt" title "rozklad wieku: m = 1" with  lp, \
"23/proces0_symulacja0_3.txt" title "rozklad wieku: m = 2" with  lp,\
"26/proces0_symulacja0_3.txt" title "rozklad wieku: m = 3" with  lp
set terminal png
replot
pause -1