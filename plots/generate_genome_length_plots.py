import os
import common


common.check_gnuplot_availability()

os.chdir("GenomeLength")

default_binary_path: str = "../../penna-model"

for genome_length in ("32", "64", "128"):
    common.run_simulation_using_binary(default_binary_path + genome_length, genome_length)

# common.generate_plots()
