set output "2_5_rozk_bitow.png"
set xlabel "wiek" 
set ylabel "prawdopodobienstwo"
set yrange [0:1.2]
set term png size 800, 500
#set style fill solid border -1
plot [0:20] \
"7/proces0_symulacja0_4.txt" title "rozklad bitow: R = 10" with  boxes, \
"10/proces0_symulacja0_4.txt" title "rozklad bitow: R = 8" with  boxes,\
"13/proces0_symulacja0_4.txt" title "rozklad bitow: R = 12" with  boxes
set terminal png
replot
pause -1
