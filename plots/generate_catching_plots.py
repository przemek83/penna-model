import os
import common


common.check_gnuplot_availability()

os.chdir("Catching")

for prefix in ("10_percent", "20_percent", "30_percent"):
    common.run_simulation(prefix)

# common.generate_plots()
