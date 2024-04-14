set output "dorsze_5_rozklady_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 600
set style fill solid border -1
plot [0:12] "5/33.20_rozklad_wieku.txt" title "33,20" with boxes,"5/34.00_rozklad_wieku.txt" title "34,00" with boxes,"5/34.60_rozklad_wieku.txt" title "34,60" with boxes, "5/35.20_rozklad_wieku.txt" title "35,20" with boxes
set terminal png
replot
replot
pause -1

