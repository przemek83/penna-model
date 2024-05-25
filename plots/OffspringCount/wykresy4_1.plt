set output "1_4_1_rozk_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 500
set logscale y 
#set style fill solid border -1
plot [0:20] \
"1/proces0_symulacja0_3.txt" title "rozklad wieku: N_0 = 30000, b = 4" with  lp, \
"4/proces0_symulacja0_3.txt" title "rozklad wieku: N_0 = 30000, b = 2" with  lp,\
"7/proces0_symulacja0_3.txt" title "rozklad wieku: N_0 = 300000, b = 3" with  lp
set terminal png
replot
pause -1