set output "4_1_umierajace_przezywajace.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: m = 1" with lines,\
"7/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: m = 1", \
"22/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: m = 2" with lines,\
"22/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: m = 2", \
"25/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: m = 3" with lines,\
"25/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: m = 3"
set terminal png
replot
replot
pause -1
