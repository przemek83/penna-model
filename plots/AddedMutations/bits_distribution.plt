load "../init.plt"
set output "bits_distribution.png"
set xlabel "bit" 
set ylabel "percent of 1s"
set yrange [0:1.2]
plot "added_1_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 1 added" with lines, \
     "added_2_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 2 added" with lines, \
     "added_3_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 3 added" with lines
load "../finalize.plt"
