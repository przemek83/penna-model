"""Module generating plots for genome length category."""

import os
import common


common.check_gnuplot_availability()

os.chdir("GenomeLength")

DEFAULT_BINARY_PATH: str = "../../penna-model"

for genome_length in ("32", "64", "128"):
    common.run_simulation_using_binary(DEFAULT_BINARY_PATH + genome_length, genome_length)

common.generate_plots()
