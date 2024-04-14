set output "4_2_zycia.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:3 title "przyrost: m = 1" with lines,\
"7/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: m = 1", \
"22/proces0_symulacja0_1.txt" using 1:3 title "przyrost: m = 2" with lines,\
"22/proces0_symulacja0_1.txt" using 1:5 with lines title "m = 2", \
"25/proces0_symulacja0_1.txt" using 1:3 title "przyrost: m = 3" with lines,\
"25/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: m = 3"
set terminal png
replot
pause -1
