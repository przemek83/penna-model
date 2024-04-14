set output "0_8_dopasowanie_gompertz_a.png"
set xlabel "rok" 
set ylabel "prawdopodobienstwo"
set logscale y
set xrange [0:23]
set yrange [:1.2]
#f(x) = 0.0281*(1.2024**x)
#f(x) = b*(c**x)
set term png size 400, 300
#fit f(x) "8/proces0_symulacja0_5.txt" using 1:2 via b,c
#unset xrange
#set term png size 800, 500
#plot  "7/proces0_symulacja0_5.txt" with lp,
plot  "8/proces0_symulacja0_5.txt" title "zgony: genom 64 bitowy" with lp
#set terminal png
replot
#pause -1
