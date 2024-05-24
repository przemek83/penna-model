set output "0_4_rozk_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 500
#set style fill solid border -1
set logscale y 
plot [0:26] \
"7/proces0_symulacja0_3.txt" title "rozklad wieku: genom 32 bitowy" with  lp, \
"8/proces0_symulacja0_3.txt" title "rozklad wieku: genom 64 bitowy" with  lp,\
"9/proces0_symulacja0_3.txt" title "rozklad wieku: genom 128 bitowy" with  lp
set terminal png
replot
pause -1