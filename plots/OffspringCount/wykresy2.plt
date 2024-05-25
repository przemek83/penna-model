set output "1_2_zycia.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"1/proces0_symulacja0_1.txt" using 1:3 title "przyrost: N_0 = 30000, b = 4" with lines,\
"1/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: N_0 = 30000, b = 4", \
"4/proces0_symulacja0_1.txt" using 1:3 title "przyrost: N_0 = 30000, b = 2" with lines,\
"4/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: N_0 = 30000, b = 2", \
"7/proces0_symulacja0_1.txt" using 1:3 title "przyrost: N_0 = 300000, b = 3" with lines,\
"7/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: N_0 = 300000, b = 3"
set terminal png
replot
pause -1
