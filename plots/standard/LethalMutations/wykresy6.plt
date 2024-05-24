set output "3_6_gompertz.png"
set xlabel "wiek" 
set ylabel "prawdopodobienstwo"
set term png size 800, 500
set yrange [0:1.2]
#set style fill solid border -1
plot [0:20] \
"7/proces0_symulacja0_5.txt" title "rozklad bitow: T = 4" with  boxes, \
"16/proces0_symulacja0_5.txt" title "rozklad bitow: T = 3" with  boxes,\
"19/proces0_symulacja0_5.txt" title "rozklad bitow: T = 2" with  boxes
set terminal png
replot
pause -1
