"""Module providing common functions for use by generators."""

import subprocess
import os
import sys


def check_gnuplot_availability():
    """Check if Gnuplot tool is available."""
    result = subprocess.run("gnuplot -V", shell=True, check=False, stdout = subprocess.DEVNULL)
    try:
        result.check_returncode()
    except subprocess.CalledProcessError:
        print("Problem when calling 'gnuplot' command. Gnuplot not available?")
        sys.exit(1)


def generate_plots():
    """Generate plots for all .plt files found in current path."""
    for path in os.listdir("."):
        if path.endswith(".plt"):
            gnuplot_cmd: str = "gnuplot -c " + path
            subprocess.run(gnuplot_cmd, shell=True, check=False)


def _run_binary(binary: str, config: str, prefix: str) -> subprocess.CompletedProcess:
    result = subprocess.run(
        binary + " " + config + " -p " + prefix,
        shell=True,
        check=False)
    return result


def _check_result(result: subprocess.CompletedProcess, binary: str):
    try:
        result.check_returncode()
    except subprocess.CalledProcessError:
        print("Problem when calling '" + binary + "' command. "
              "Binary not available?")
        sys.exit(1)

def run_simulation_using_binary(binary: str, prefix: str):
    """Run simulation with given binary name and output files prefix."""
    config_name: str = "config.yaml"
    result = _run_binary(binary, config_name, prefix)
    _check_result(result, binary)


def run_simulation(prefix: str):
    """Run simulation using default binary name and given output files prefix."""
    config_name: str = "config.yaml"
    if prefix:
        config_name: str = prefix + "_" + config_name

    binary: str = "penna-model"
    result = _run_binary("../../" + binary, config_name, prefix)

    _check_result(result, binary)
