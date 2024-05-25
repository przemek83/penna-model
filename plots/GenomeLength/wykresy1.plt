set output "0_1_umierajace_przezywajace.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: genom 32 bitowy" with lines,\
"7/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: genom 32 bitowy", \
"8/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: genom 64 bitowy" with lines,\
"8/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: genom 64 bitowy", \
"9/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: genom 128 bitowy" with lines,\
"9/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: genom 128 bitowy"
set terminal png
replot
replot
pause -1
