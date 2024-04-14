set output "dorsze_4_rozklady_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 600
set style fill solid border -1
plot [0:12] "4/38.40_rozklad_wieku.txt" title "38,40" with boxes,"4/39.20_rozklad_wieku.txt" title "39,20" with boxes,"4/40.00_rozklad_wieku.txt" title "40,00" with boxes, "4/40.80_rozklad_wieku.txt" title "40,80" with boxes, "4/41.60_rozklad_wieku.txt" title "41,60" with boxes
set terminal png
replot
replot
pause -1

