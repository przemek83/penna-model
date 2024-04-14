set output "4_5_rozk_bitow.png"
set xlabel "wiek" 
set ylabel "prawdopodobienstwo"
set yrange [0:1.2]
set term png size 800, 500
#set style fill solid border -1
plot [0:20] \
"7/proces0_symulacja0_4.txt" title "rozklad bitow: m = 1" with  boxes, \
"22/proces0_symulacja0_4.txt" title "rozklad bitow: m = 2" with  boxes,\
"25/proces0_symulacja0_4.txt" title "rozklad bitow: m = 3" with  boxes
set terminal png
replot
pause -1
