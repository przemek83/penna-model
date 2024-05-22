import subprocess
import os
import sys

print(os.getcwd())
os.chdir("basic")
print(os.getcwd())

result = subprocess.run("gnuplot -V", shell=True)
try:
    result.check_returncode()
except subprocess.CalledProcessError:
    print("Problem when calling 'gnuplot' command. Gnuplot not available?")
    sys.exit(1)

for path in os.listdir("."):
    if path.endswith(".plt"):
        gnuplot_cmd = 'gnuplot -c ' + path
        subprocess.run(gnuplot_cmd, shell=True)