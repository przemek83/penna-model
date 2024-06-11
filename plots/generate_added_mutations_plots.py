import os
import common


common.check_gnuplot_availability()

os.chdir("AddedMutations")

for prefix in ("added_1", "added_2", "added_3"):
    common.run_simulation(prefix)

common.generate_plots()
