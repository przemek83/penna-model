import subprocess
import os
import sys

print(os.getcwd())
os.chdir("basic")
print(os.getcwd())

result = subprocess.run("gnuplots -V", shell=True)

try:
    result.check_returncode()
except subprocess.CalledProcessError:
    print("Problem when calling 'gnuplot' command. Gnuplot not available?")
    sys.exit(1)

gnuplot_cmd = 'gnuplot -c population.plt'
subprocess.run(gnuplot_cmd, shell=True)