load "../init.plt"
set output "1_1_umierajace_przezywajace.png"
set xlabel "rok" 
set ylabel "ilosc"
plot [0:150000] \
"1/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: N_0 = 30000, b = 4" with lines,\
"1/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: N_0 = 30000, b = 4", \
"4/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: N_0 = 30000, b = 2" with lines,\
"4/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: N_0 = 30000, b = 2", \
"7/proces0_symulacja0_1.txt" using 1:2 title "poczatek roku: N_0 = 300000, b = 3" with lines,\
"7/proces0_symulacja0_1.txt" using 1:4 with lines title "koniec roku: N_0 = 300000, b = 3"
load "../finalize.plt"
