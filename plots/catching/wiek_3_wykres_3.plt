set output "dorsze_4_wielkosci_populacji.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 600
plot [45000:150000] "4/38.40_statystyki.txt" title "38,40" with lines,"4/39.20_statystyki.txt" title "39,20" with lines,"4/40.00_statystyki.txt" title "40,00" with lines, "4/40.80_statystyki.txt" title "40,80" with lines, "4/41.60_statystyki.txt" title "41,60" with lines
set terminal png
replot
replot
pause -1

