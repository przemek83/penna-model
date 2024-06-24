set output "dorsze_1_rozklady_wieku.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 600
set logscale y 
#set style fill solid border -1
plot [0:12] "1/1.00_rozklad_wieku.txt" title "1,00" with lp,"1/10.00_rozklad_wieku.txt" title "10,00" with lp,"1/20.00_rozklad_wieku.txt" title "20,00" with lp, "1/30.00_rozklad_wieku.txt" title "30,00" with lp, "1/40.00_rozklad_wieku.txt" title "40,00" with lp
set terminal png
replot
replot
pause -1

