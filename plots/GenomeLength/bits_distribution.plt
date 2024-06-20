load "../init.plt"
set output "bits_distribution.png"
set xlabel "bit" 
set ylabel "percent of 1s"
set yrange [0:1.2]
set xrange [0:31]
plot "32_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 32 bit genome" with lines, \
     "64_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 64 bit genome" with lines, \
     "128_bits_distribution.txt" using (column("Bit")) : (column("Percent")) title "bits distribution - 128 bit genome" with lines
load "../finalize.plt"
