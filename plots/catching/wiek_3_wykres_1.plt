set output "dorsze_1_wielkosci_populacji.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 600
plot [45000:150000] "1/1.00_statystyki.txt" title "1,00" with lines,"1/10.00_statystyki.txt" title "10,00" with lines,"1/20.00_statystyki.txt" title "20,00" with lines, "1/30.00_statystyki.txt" title "30,00" with lines, "1/40.00_statystyki.txt" title "40,00" with lines
set terminal png
replot
replot
pause -1

