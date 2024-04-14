set output "1_6_gompertz.png"
set xlabel "wiek" 
set ylabel "prawdopodobienstwo"
set term png size 800, 500
set yrange [0:1.2]
set style fill solid border -1
plot [0:20] \
"1/proces0_symulacja0_5.txt" title "rozklad bitow: N_0 = 30000, b = 4" with  boxes, \
"7/proces0_symulacja0_5.txt" title "rozklad bitow: N_0 = 300000, b = 3" with  boxes,\
"4/proces0_symulacja0_5.txt" title "rozklad bitow: N_0 = 30000, b = 2" with  boxes	
set terminal png
replot
pause -1
