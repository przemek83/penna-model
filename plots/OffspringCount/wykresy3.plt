load "../init.plt"
set output "1_3_rodziny.png"
set xlabel "rok" 
set ylabel "ilosc"
set logscale y 
set logscale x 
plot  \
"1/proces0_symulacja0_6.txt" title "rodziny: N_0 = 30000, b = 4" with lines, \
"4/proces0_symulacja0_6.txt" title "rodziny: N_0 = 30000, b = 2" with lines, \
"7/proces0_symulacja0_6.txt" title "rodziny: N_0 = 300000, b = 3" with lines
load "../finalize.plt"