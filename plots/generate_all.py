"""Module generating plots for all categories."""

import importlib
import os
import sys


def _generate(category: str):
    print("Generating " + category)
    file = "generate_" + category + "_plots"
    importlib.import_module(file)
    os.chdir(sys.path[0])
    print("Finished " + category)

categories = ("basic", "added_mutations", "catching", "genome_length",
              "lethal_mutations", "offspring_count", "reproduction_age")

for current_category in categories:
    _generate(current_category)
