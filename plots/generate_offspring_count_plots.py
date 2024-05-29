import subprocess
import os
import sys
import common

common.check_gnuplot_availability()

os.chdir("OffspringCount")

for prefix in ("count_2", "count_3", "count_4"):
    config_name = prefix + "_config.yaml"
    result = subprocess.run("../../penna-model -p " + prefix + " " + config_name, shell=True)
    try:
        result.check_returncode()
    except subprocess.CalledProcessError:
        print("Problem when calling 'penna-model' command. Binary not available?")
        sys.exit(1)

common.generate_plots()
