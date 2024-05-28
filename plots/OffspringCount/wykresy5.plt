load "../init.plt"
set output "bits_distribution.png"
set xlabel "bit" 
set ylabel "percent of 1s"
set yrange [0:1.2]
plot "count_2_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 2 x offspring" with boxes, \
     "count_3_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 3 x offspring" with boxes, \
     "count_4_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 4 x offspring" with boxes
load "../finalize.plt"
