set output "dorsze_3_wielkosci_populacji.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 600
plot [45000:150000] "3/1.00_statystyki.txt" title "1,00" with lines,"3/10.00_statystyki.txt" title "10,00" with lines,"3/20.00_statystyki.txt" title "20,00" with lines, "3/30.00_statystyki.txt" title "30,00" with lines, "3/40.00_statystyki.txt" title "40,00" with lines, "3/50.00_statystyki.txt" title "50,00" with lines
set terminal png
replot
replot
pause -1

