import os
import common


common.check_gnuplot_availability()

os.chdir("ReproductionAge")

for prefix in ("age_8", "age_10", "age_12"):
    common.run_simulation(prefix)

common.generate_plots()
