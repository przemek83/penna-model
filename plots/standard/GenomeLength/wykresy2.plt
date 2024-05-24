set output "0_2_zycia.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:3 title "przyrost: genom 32 bitowy" with lines,\
"7/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: genom 32 bitowy", \
"8/proces0_symulacja0_1.txt" using 1:3 title "przyrost: genom 64 bitowy" with lines,\
"8/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: genom 64 bitowy", \
"9/proces0_symulacja0_1.txt" using 1:3 title "przyrost: genom 128 bitowy" with lines,\
"9/proces0_symulacja0_1.txt" using 1:5 with lines title "zgony: genom 128 bitowy"
set terminal png
replot
pause -1
