set output "0_5_rozk_bitow.png"
set xlabel "wiek" 
set ylabel "prawdopodobienstwo"
set yrange [0:1.2]
set term png size 800, 500
#set style fill solid border -1
plot [0:25] \
"7/proces0_symulacja0_4.txt" title "rozklad bitow: genom 32 bitowy" with  boxes, \
"8/proces0_symulacja0_4.txt" title "rozklad bitow: genom 64 bitowy" with  boxes,\
"9/proces0_symulacja0_4.txt" title "rozklad bitow: genom 128 bitowy" with  boxes
set terminal png
replot
pause -1