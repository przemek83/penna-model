## Table of content
- [About project](#about-project)
- [Building](#building)
- [Usage](#usage)
- [Licensing](#licensing)
- [Testing](#testing)
- [Potential further improvements](#potential-further-improvements)


# About project
[WIP] - Work In Progress

Implementation of the Penna population aging model with multiple configuration options. The program runs a given number of simulations in a sequential or parallel manner and saves the averages into output files. The initial implementation was part of my master thesis and was created in the first decade of the 21st century. Reworked heavily in the years 2023–2024 and published as open source.

The implementation is written in C++17, with a focus on speed and configurability.
  
# Building
Clone and use CMake directly or via any CMake supporting IDE. CMake **should**:
+ configure everything automatically,
+ compile and create binaries.

As a result of compilation binary for simulations and binary for testing should be created.

## Used tools and libs for C++ part
| Tool |  Windows | Lubuntu |
| --- | --- | --- |
| OS version | 10 22H2 | 22.04 |
| GCC | 11.2.0 | 11.3.0 |
| CMake | 3.25.0 | 3.25.0 |
| Git | 2.38.1 | 2.34.1 |
| Qt Creator | 10.0.2 | 10.0.2 |

## Used tools and libs for Python part
TODO

# Usage
TODO

## Parameters
 Example `config.yaml` configuration file:

    population:
      initial: 3000
      max: 50000
    years: 5000
    mutations:
      added: 1
      lethal: 4
      initial: 4
    reproductionAge: 10
    offspring:
      chance: 100
      count: 3
    simulations: 2
    catching:
      percent: 0
      fromYear: 1000
      fromAge: 4

Following parameters are supported via configuration file:
| Parameter | sub-parameter |  Default value | Description |
| --- | --- | --- | --- |
| population | | |
| | initial | 3000 | Size of population ans simulation start. |
| | max | 50000 | Maximal allowed population. Used in Verhulst factor calculations. |
| years | | 5000 | Length of simulation in years. |
| mutations | | |
| | added | 1 | Mutations applied on offspring. |
| | lethal | 4 | Number of mutations becoming lethal when accumulated. |
| | initial | 4 | Number of mutations in genome of population on start. |
| reproductionAge | | 10 | Age in which individual stars to have offspring. |
| offspring | | |
| | chance | 100 | TODO |
| | count | 3 | TODO |
| simulations | | 2 | TODO |
| catching | | |
| | percent | 0 | TODO |
| | fromYear | 1000 | TODO |
| | fromAge | 4 | TODO |

## Sequential run
TODO

## Parallel run
TODO

## Progress indicators
TODO

# Plots
TODO

# Licensing
Software is licenced under MIT licence.

The project uses the following software and licenses:    
* yaml-cpp TODO,    
* Catch2 TODO,
* Argparse TODO.

# Testing
TODO

# Potential further improvements
* Upgrade code to use C++20/23.
* Measure and increase test coverage.
* Setup static analysis (CppCheck, Clang Tidy, Clazy).
* Make size of genome configurable (use something different than std::bitset).


TODO