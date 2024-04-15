# Table of content
- [Table of content](#table-of-content)
- [About project](#about-project)
- [Building](#building)
   * [Used tools and libs for C++ part](#used-tools-and-libs-for-c-part)
   * [Used tools and libs for Python part](#used-tools-and-libs-for-python-part)
- [Usage](#usage)
   * [Parameters](#parameters)
   * [Sequential run](#sequential-run)
   * [Parallel run](#parallel-run)
   * [Progress indicators](#progress-indicators)
   * [Genome length](#genome-length)
- [Plots](#plots)
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

The following parameters are supported via the configuration file:
| Parameter | Sub-parameter | Default value | Description |
| --- | --- | --- | --- |
| population | | |
| | initial | 3000 | Size of population on simulation start. |
| | max | 50000 | Maximal allowed population. Value used in Verhulst factor calculations. |
| years | | 5000 | Length of simulation in years. |
| mutations | | |
| | added | 1 | Mutations applied to offspring. |
| | lethal | 4 | Number of mutations becoming lethal when accumulated. |
| | initial | 4 | Number of mutations in genome for eacvh individual on simulation start. |
| reproductionAge | | 10 | Age in which individual stars have offspring. |
| offspring | | |
| | chance | 100 | Chance for having offspring each year. |
| | count | 3 | Number of new individuals born. |
| simulations | | 2 | Number of simulations to run. Results are created using averages values. |
| catching | | |
| | percent | 0 | Percent of individuals caught each year. Zero means no catching. |
| | fromYear | 1000 | Year when catching starts. |
| | fromAge | 4 | Age from which individuals are taken into consideration when catching is applied. |

## Sequential run
TODO

## Parallel run
TODO

## Progress indicators
TODO

## Genome length
Genome length is hard-coded in the `Config::Params` structure at field `bits_`. Its value equals to `64`. To modify genome length, change the value of `bits_` to a different one and rebuild the binary. As of April 2024, there is no plan to make it configurable in yaml, as resigning from `std::bitset` and introducing something more flexible like `std::vector<bool>` impacts greatly performance.

# Plots
TODO

# Licensing
Software is licenced under MIT licence.

The project uses the following open-source software:    
* Yaml-cpp is a YAML parser library for C++ with a MIT license (more info: https://github.com/jbeder/yaml-cpp),    
* Catch2 is a testing framework for C++ with a BSL-1.0 license (more info: https://github.com/catchorg/Catch2).
* Argparse is an argument parser for C++ with a MIT license (more info: https://github.com/p-ranav/argparse).

# Testing
TODO

# Potential further improvements
* Upgrade code to use C++20/23.
* Measure and increase test coverage.
* Setup static analysis (CppCheck, Clang Tidy, Clazy).
* Make size of genome configurable (use something different than std::bitset).


TODO