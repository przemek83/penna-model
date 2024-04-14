set output "3_2_zycia.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:3 title "przyrost: T = 4" with lines,\
"7/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: T = 4", \
"16/proces0_symulacja0_1.txt" using 1:3 title "przyrost: T = 3" with lines,\
"16/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: T = 3", \
"19/proces0_symulacja0_1.txt" using 1:3 title "przyrost: T = 2" with lines,\
"19/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: T = 2"
set terminal png
replot
pause -1
