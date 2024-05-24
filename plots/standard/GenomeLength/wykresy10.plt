set output "0_10_fazy_populacji.png"
set xlabel "wiek" 
set ylabel "ilosc"
set term png size 800, 500
#set style fill solid border -1
plot [10:170] \
"7/proces0_symulacja0_1.txt" using 1:2 title "rozmiar populacji: genom 32 bitowy" with  lines, \
"8/proces0_symulacja0_1.txt" using 1:2 title "rozmiar populacji: genom 64 bitowy" with  lines,\
"9/proces0_symulacja0_1.txt" using 1:2 title "rozmiar populacji: genom 128 bitowy" with  lines
set terminal png
replot
pause -1
