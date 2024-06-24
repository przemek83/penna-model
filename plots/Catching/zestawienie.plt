set output "zestawienie.png"
set xlabel "procent odlawianych" 
set ylabel "ilosc"
set term png size 800, 600
plot "zestawienie_doswiadczenie_1.txt" title "Doswiadczenie 1" smooth bezier with lines,"zestawienie_doswiadczenie_2.txt" title "Doswiadczenie 2" smooth bezier with lines,"zestawienie_doswiadczenie_3.txt" title "Doswiadczenie 3" smooth bezier with lines
set terminal png
replot
replot
pause -1