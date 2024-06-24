set output "dorsze_2_wielkosci_populacji.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 600
plot [45000:150000] "2/1.00_statystyki.txt" title "1,00" with lines,"2/10.00_statystyki.txt" title "10,00" with lines,"2/20.00_statystyki.txt" title "20,00" with lines, "2/30.00_statystyki.txt" title "30,00" with lines, "2/34.00_statystyki.txt" title "34,00" with lines
set terminal png
replot
replot
pause -1

