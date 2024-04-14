set output "2_4_2_rozk_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 500
#set style fill solid border -1
set logscale y 
plot [0:25] \
"8/proces0_symulacja0_3.txt" title "rozklad wieku: R = 10" with  lp, \
"11/proces0_symulacja0_3.txt" title "rozklad wieku: R = 8" with  lp,\
"14/proces0_symulacja0_3.txt" title "rozklad wieku: R = 12" with  lp
set terminal png
replot
pause -1