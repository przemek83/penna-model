set output "3_1_umierajace_przezywajace.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 500
plot [0:150000] \
"7/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: T = 4" with lines,\
"7/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: T = 4", \
"16/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: T = 3" with lines,\
"16/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: T = 3", \
"19/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: T = 2" with lines,\
"19/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: T = 2"
set terminal png
replot
replot
pause -1
