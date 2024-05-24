set output "2_2_zycia.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:3 title "przyrost: R = 10" with lines,\
"7/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: R = 10", \
"10/proces0_symulacja0_1.txt" using 1:3 title "przyrost: R = 8" with lines,\
"10/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: R = 8", \
"13/proces0_symulacja0_1.txt" using 1:3 title "przyrost: R = 12" with lines,\
"13/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: R = 12"
set terminal png
replot
pause -1
