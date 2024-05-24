set output "2_6_gompertz.png"
set xlabel "wiek" 
set ylabel "prawdopodobienstwo"
set term png size 800, 500
set yrange [0:1.2]
set style fill solid border -1
plot [0:21] \
"10/proces0_symulacja0_5.txt" title "rozklad bitow: R = 8" with  boxes,\
"7/proces0_symulacja0_5.txt" title "rozklad bitow: R = 10" with  boxes,\
"13/proces0_symulacja0_5.txt" title "rozklad bitow: R = 12" with  boxes
set terminal png
replot
pause -1
