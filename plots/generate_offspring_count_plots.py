"""Module generating plots for offspring category."""

import os
import common


common.check_gnuplot_availability()

os.chdir("OffspringCount")

for prefix in ("count_2", "count_3", "count_4"):
    common.run_simulation(prefix)

common.generate_plots()
