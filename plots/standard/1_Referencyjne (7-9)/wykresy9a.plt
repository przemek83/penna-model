set output "0_9_dopasowanie_gompertz_a.png"
set xlabel "rok" 
set ylabel "ilosc"
set logscale y
set xrange [0:25]
set yrange [:1.2]
#f(x) = 0.0281*(1.2024**x)
#f(x) = b*(c**x)
set term png size 400, 300
#fit f(x) "9/proces0_symulacja0_5.txt" using 1:2 via b,c
#unset xrange
#set term png size 800, 500
#plot  "7/proces0_symulacja0_5.txt" with lp,
plot "9/proces0_symulacja0_5.txt" title "zgony: genom 128 bitowy" with lp
#set terminal png
replot
#pause -1
