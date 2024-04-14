set output "0_8_dopasowanie_gompertz.png"
set xlabel "rok" 
set ylabel "prawdopodobienstwo"
#set logscale
set xrange [12:22]
#f(x) = 0.0281*(1.2024**x)
f(x) = b*(c**x)
set term png size 800, 500
fit f(x) "8/proces0_symulacja0_5.txt" using 1:2 via b,c
unset xrange
#set term png size 800, 500
#plot  "7/proces0_symulacja0_5.txt" with lp,
plot  [11:23][0:1.2] "8/proces0_symulacja0_5.txt" title "zgony: genom 64 bitowy" with lp, f(x) title "f(x) = 0.0134402*1.21474^x" with lines
#set terminal png
replot
#pause -1
