set output "2_1_umierajace_przezywajace.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: R = 10" with lines,\
"7/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: R = 10", \
"10/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: R = 8" with lines,\
"10/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: R = 8", \
"13/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: R = 12" with lines,\
"13/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: R = 12"
set terminal png
replot
replot
pause -1
