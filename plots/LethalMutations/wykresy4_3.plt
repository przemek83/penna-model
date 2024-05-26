set output "3_4_3_rozk_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 500
set logscale y 
#set style fill solid border -1
plot [0:27] \
"9/proces0_symulacja0_3.txt" title "rozklad wieku: T = 4" with  lp, \
"18/proces0_symulacja0_3.txt" title "rozklad wieku: T = 3" with  lp,\
"21/proces0_symulacja0_3.txt" title "rozklad wieku: T = 2" with  lp
set terminal png
replot
pause -1