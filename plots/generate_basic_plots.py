import os
import common


common.check_gnuplot_availability()

os.chdir("basic")

common.run_simulation("")

common.generate_plots()
