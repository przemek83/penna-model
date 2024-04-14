set output "0_9_dopasowanie_gompertz.png"
set xlabel "rok" 
set ylabel "prawdopodobienstwo"
#set logscale
set xrange [13:25]
#f(x) = 0.0281*(1.2024**x)
f(x) = b*(c**x)
set term png size 800, 500
fit f(x) "9/proces0_symulacja0_5.txt" using 1:2 via b,c
unset xrange
#set term png size 800, 500
#plot  "7/proces0_symulacja0_5.txt" with lp,
plot  [12:26][0:1.2] "9/proces0_symulacja0_5.txt" title "zgony: genom 128 bitowy" with lp, f(x) title "f(x) = 0.0118518*1.19136^x" with lines
#set terminal png
replot
#pause -1
