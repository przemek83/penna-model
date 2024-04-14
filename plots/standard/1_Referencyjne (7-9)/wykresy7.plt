set output "0_7_dopasowanie_gompertz.png"
set xlabel "rok" 
set ylabel "prawdopodobienstwo"
#set logscale
set xrange [8:20]
#f(x) = 0.0281*(1.2024**x)
f(x) = b*(c**x)
set term png size 800, 500
fit f(x) "7/proces0_symulacja0_5.txt" using 1:2 via b,c
unset xrange
#set term png size 800, 500
#plot  "7/proces0_symulacja0_5.txt" with lp,
plot  [8:19][0:1.2] "7/proces0_symulacja0_5.txt" title "procent zgonow: genom 32 bitowy" with lp, f(x) title "f(x) = 0.0296519*1.19862^x" with lines
#set terminal png
replot
#pause -1
