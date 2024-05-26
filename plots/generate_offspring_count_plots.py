import subprocess
import os
import sys


os.chdir("OffspringCount")

for prefix in ("count_2", "count_3", "count_4"):
    config_name = prefix + "_config.yaml"
    result = subprocess.run("../../penna-model -p " + prefix + " " + config_name, shell=True)
    try:
        result.check_returncode()
    except subprocess.CalledProcessError:
        print("Problem when calling 'penna-model' command. Binary not available?")
        sys.exit(1)

# result = subprocess.run("gnuplot -V", shell=True)
# try:
#     result.check_returncode()
# except subprocess.CalledProcessError:
#     print("Problem when calling 'gnuplot' command. Gnuplot not available?")
#     sys.exit(1)

# for path in os.listdir("."):
#     if path.endswith(".plt"):
#         gnuplot_cmd = 'gnuplot -c ' + path
#         subprocess.run(gnuplot_cmd, shell=True)