import subprocess
import os
import sys


def check_gnuplot_availability():
    result = subprocess.run("gnuplot -V", shell=True, check=False)
    try:
        result.check_returncode()
    except subprocess.CalledProcessError:
        print("Problem when calling 'gnuplot' command. Gnuplot not available?")
        sys.exit(1)


def generate_plots():
    for path in os.listdir("."):
        if path.endswith(".plt"):
            gnuplot_cmd: str = 'gnuplot -c ' + path
            subprocess.run(gnuplot_cmd, shell=True, check=False)


def run_simulation(prefix: str):
    config_name: str = prefix + "_config.yaml"
    result = subprocess.run(
        "../../penna-model -p " + prefix + " " + config_name,
        shell=True,
        check=False)
    try:
        result.check_returncode()
    except subprocess.CalledProcessError:
        print("Problem when calling 'penna-model' command. "
              "Binary not available?")
        sys.exit(1)
