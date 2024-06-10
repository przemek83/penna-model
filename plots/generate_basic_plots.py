import subprocess
import os
import sys
import common


common.check_gnuplot_availability()

os.chdir("basic")

result = subprocess.run("../../penna-model config.yaml",
                        shell=True,
                        check=False)
try:
    result.check_returncode()
except subprocess.CalledProcessError:
    print("Problem when calling 'penna-model' command. Binary not available?")
    sys.exit(1)

common.generate_plots()
