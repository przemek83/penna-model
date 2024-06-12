load "../init.plt"
set output "bits_distribution.png"
set xlabel "bit" 
set ylabel "percent of 1s"
set yrange [0:1.2]
plot "lethal_2_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 2 lethal" with lines, \
     "lethal_3_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 3 lethal" with lines, \
     "lethal_4_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 4 lethal" with lines
load "../finalize.plt"
