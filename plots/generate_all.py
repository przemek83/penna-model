import importlib
import os
import sys


def generate(category: str):
    print("Generating " + category)
    file = "generate_" + category + "_plots"
    importlib.import_module(file)
    os.chdir(sys.path[0])
    print("Finished " + category)

categories = ("basic", "added_mutations", "catching", "genome_length", "lethal_mutations", "offspring_count", "reproduction_age")

for category in categories:
    generate(category)
