set output "dorsze_6_wielkosci_populacji.png"
set xlabel "rok" 
set ylabel "ilosc"
set term png size 800, 600
plot [45000:150000] "6/50.20_statystyki.txt" title "50,20" with lines,"6/50.80_statystyki.txt" title "50,80" with lines,"6/51.20_statystyki.txt" title "51,20" with lines, "6/51.60_statystyki.txt" title "51,60" with lines
set terminal png
replot
replot
pause -1

