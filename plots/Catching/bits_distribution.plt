load "../init.plt"
set output "bits_distribution.png"
set xlabel "bit" 
set ylabel "percent of 1s"
set yrange [0:1.2]
plot "10_percent_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 10% catched" with lines, \
     "15_percent_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 15% catched" with lines, \
     "20_percent_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 20% catched" with lines
load "../finalize.plt"
