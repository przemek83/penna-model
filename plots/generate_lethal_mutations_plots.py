import os
import common


common.check_gnuplot_availability()

os.chdir("LethalMutations")

for prefix in ("lethal_2", "lethal_3", "lethal_4"):
    common.run_simulation(prefix)

common.generate_plots()
