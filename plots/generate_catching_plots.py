"""Module generating plots for catching category."""

import os
import common


common.check_gnuplot_availability()

os.chdir("Catching")

for prefix in ("10_percent", "15_percent", "20_percent"):
    common.run_simulation(prefix)

common.generate_plots()
