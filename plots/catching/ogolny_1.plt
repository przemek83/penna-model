set output "poczatkowe_populacje_dorsze.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 600
plot [0:50000] "1/1.00_statystyki.txt" title "Doswiadczenie 1, odlowy 1,00" with lines,"2/9.00_statystyki.txt" title "Doswiadczenie 2, odlowy 9,00" with lines,"3/18.00_statystyki.txt" title "Doswiadczenie 3, odlowy 18,00" with lines, "4/39.20_statystyki.txt" title "Doswiadczenie 4, odlowy 39,20" with lines, "5/34.60_statystyki.txt" title "Doswiadczenie 5, odlowy 34,60" with lines, "6/50.60_statystyki.txt" title "Doswiadczenie 6, odlowy 50,60" with lines  
set terminal png
replot
replot
pause -1

