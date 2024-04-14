set output "dorsze_5_wielkosci_populacji.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 600
plot [45000:150000] "5/33.20_statystyki.txt" title "33,20" with lines,"5/34.00_statystyki.txt" title "34,00" with lines,"5/34.60_statystyki.txt" title "34,60" with lines, "5/35.20_statystyki.txt" title "35,20" with lines
set terminal png
replot
replot
pause -1

