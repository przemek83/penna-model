load "../init.plt"
set output "bits_distribution.png"
set xlabel "bit" 
set ylabel "percent of 1s"
set yrange [0:1.2]
plot "age_8_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - age 8" with lines, \
     "age_10_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - age 10" with lines, \
     "age_12_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - age 12" with lines
load "../finalize.plt"
