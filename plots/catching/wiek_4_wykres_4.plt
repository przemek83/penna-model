set output "dorsze_6_rozklady_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 600
set style fill solid border -1
plot [0:12] "6/50.20_rozklad_wieku.txt" title "50,20" with boxes,"6/50.80_rozklad_wieku.txt" title "50,80" with boxes,"6/51.20_rozklad_wieku.txt" title "51,20" with boxes, "6/51.60_rozklad_wieku.txt" title "51,60" with boxes
set terminal png
replot
replot
pause -1

