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
    config_name: str = "config.yaml"
    if prefix:
        config_name: str = prefix + "_" + config_name
        
    result = subprocess.run(
        "../../penna-model " + config_name + " -p " + prefix,
        shell=True,
        check=False)
    try:
        result.check_returncode()
    except subprocess.CalledProcessError:
        print("Problem when calling 'penna-model' command. "
              "Binary not available?")
        sys.exit(1)
