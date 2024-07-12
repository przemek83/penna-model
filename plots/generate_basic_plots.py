"""Module generating plots for basic category."""

import os
import common


common.check_gnuplot_availability()

os.chdir("Basic")

common.run_simulation("")

common.generate_plots()
