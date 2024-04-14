set output "dorsze_3_rozklady_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 600
set logscale y 
#set style fill solid border -1
plot [0:12] "3/1.00_rozklad_wieku.txt" title "1,00" with lp,"3/10.00_rozklad_wieku.txt" title "10,00" with lp,"3/20.00_rozklad_wieku.txt" title "20,00" with lp, "3/30.00_rozklad_wieku.txt" title "30,00" with lp, "3/40.00_rozklad_wieku.txt" title "40,00" with lp, "3/50.00_rozklad_wieku.txt" title "50,00" with lp
set terminal png
replot
replot
pause -1

