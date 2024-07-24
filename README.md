[![Build & test on Linux ](https://github.com/przemek83/penna-model/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/przemek83/penna-model/actions/workflows/cmake-single-platform.yml)
[![CodeQL](https://github.com/przemek83/penna-model/actions/workflows/codeql.yml/badge.svg)](https://github.com/przemek83/penna-model/actions/workflows/codeql.yml)
[![Pylint](https://github.com/przemek83/penna-model/actions/workflows/pylint.yml/badge.svg)](https://github.com/przemek83/penna-model/actions/workflows/pylint.yml)
[![codecov](https://codecov.io/github/przemek83/penna-model/graph/badge.svg?token=NYK9LBVOJE)](https://codecov.io/github/przemek83/penna-model)

# Table of content
- [About project](#about-project)
- [Building](#building)
   * [Used tools and libraries for the C++ part](#used-tools-and-libraries-for-the-c-part)
   * [Used tools for plots part](#used-tools-for-plots-part)
- [Usage](#usage)
   * [Parameters](#parameters)
   * [Sequential run](#sequential-run)
   * [Parallel run](#parallel-run)
   * [Progress indicators](#progress-indicators)
      + [Overall progress](#overall-progress)
      + [Sequential progress](#sequential-progress)
   * [Genome length](#genome-length)
- [Plots](#plots)
   * [Example basic plots](#example-basic-plots)
   * [Example catching plots](#example-catching-plots)
   * [Example genome length plots](#example-genome-length-plots)
   * [Example reproduction age plots](#example-reproduction-age-plots)
- [Licensing](#licensing)
- [Testing](#testing)

# About project
Implementation of the Penna population aging model with multiple configuration options. The program runs a given number of simulations in a sequential or parallel manner and saves the averages into output files. The initial implementation was part of my master thesis and was created in the first decade of the 21st century. Reworked heavily in the years 2023–2024 and published as open source.

The implementation is written in C++17, with a focus on speed and configurability.
  
# Building
Clone and use CMake directly or via any IDE supporting it. CMake **should**:
+ configure everything automatically,
+ compile and create binaries.

As a result of compilation, binary for simulations and binary for testing should be created.

## Used tools and libraries for the C++ part
| Tool |  Windows | Lubuntu |
| --- | --- | --- |
| OS version | 10 22H2 | 22.04 |
| GCC | 11.2.0 | 11.3.0 |
| CMake | 3.25.0 | 3.25.0 |
| Git | 2.38.1 | 2.34.1 |
| yaml-cpp | 0.8.0 | 0.8.0 |
| Catch2 | 3.3.0 | 3.3.0 | 
| Argparse | 3.0 | 3.0 |

## Used tools for plots part
| Tool | Lubuntu |
| --- | --- |
| OS version | 22.04 |
| Python | 3.11.4 |
| Gnuplot | 5.4 patchlevel 2 |
| Visual Studio Code | 1.80.0 |

# Usage
Call `penna-model` binary and pass appropriate configuration file as parameter:

    penna-model config.yaml

Omitting the configuration parameter will result in default behavior, and the application will try to load the configuration file `config.yaml` from the local directory. Use the optional prefix parameter (`-p, --prefix`) to store simulation output data with other than default names. Default names for output data files start with the word `averages`.

To show help, use the common option `-h` or the more verbose version `--help`. The application should display the following information:

    Usage: penna-model [--help] [--prefix VAR] config

    Implementation of Penna model of population aging.

    Positional arguments:
    config        name of config file to use [nargs=0..1] [default: "config.yaml"]

    Optional arguments:
    -h, --help    shows help message and exits 
    -p, --prefix  prefix used for output file names [nargs=0..1] [default: "averages"]

    Run outputs results into .txt files. You may use scripts placed in the 'plots' directory to generate plots.

## Parameters
 Example `config.yaml` configuration file:
```yaml
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
```
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
| | initial | 4 | Number of mutations in genome for each individual on simulation start. |
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
To run simulation after simulation instead all at once (in sequence) use `Runner::runSequential` method in `main` function replacing line
```cpp
const std::vector<SimulationData> simulationsData{runner.runParallel()};
```
with
```cpp
const std::vector<SimulationData> simulationsData{runner.runSequential()}; 
```
Computation will be executed using one thread only.

## Parallel run
By default, simulations are run in parallel using method `Runner::runParallel`. For those purposes, the functionality of `std::future` combined with `std::async` is used. Launching all simulations at once might take additional workload when there are more simulations than threads on a machine. This behavior might be improved by modifying the mentioned `Runner::runParallel` and allowing it to run a limited number of simulations in parallel at once.

## Progress indicators
The implementation contains 2 optional progress callbacks available to use to visualize the current status of the run. Both are located in namespace `ProgressCallback`. 

> **_NOTE:_**  Progress callback are optional and application can run without it. Remove call of `Simulation::setProgressCallback` to not use it.

### Overall progress
The first progress callback shows overall information. The output is as follows:

    [**************************************************]
    Execution time: 10s.

The overall progress callback can be combined with both ways of execution: parallel and sequential.

### Sequential progress
The second type of progress callback is a sequential one. Example execution output is as follows:

    1/6 [**************************************************]
    2/6 [**************************************************]
    3/6 [**************************************************]
    4/6 [**************************************************]
    5/6 [**************************************************]
    6/6 [**************************************************]
    Execution time: 7s.

The sequential progress callback should be combined with sequential execution.

## Genome length
Genome length is hard-coded in the `Config::Params` structure at field `bits_`. Its value equals to `64`. To modify genome length, one can do 2 things:
- change the value of `bits_` to a different one and rebuild the binaries,
- in file CMakeLists.txt uncomment line with call to function `generate_other_genome_bit_lenghts_binaries()` and rebuild project to have 32, 64 and 128 genome length binaries.

As of April 2024, there is no plan to make it configurable in yaml. Replacing `std::bitset` with something more flexible like `std::vector<bool>` impacts performance greatly.

# Plots
Plots are generated using Gnuplot. Multiple categories of plots can be found in `plots` directory. Categories are:
| category name | generate using | Description |
| --- | --- | --- |
| AddedMutations | generate_added_mutations_plots.py | Category showing impact on simulation of different number of mutations applied to offspring. |
| Basic | generate_basic_plots.py | Category is showing some simulation fundamentals like Eve Effect, age/bits distribution, deaths distribution with Gompertz law, phases of population (rise, fall, fluctuation, stabilization). |
| Catching | generate_catching_plots.py | Category used for visualization of impact of catching/killing of individuals according to set percent, starting year, and starting age. Used for determining what percent of the population (for example, the Atlantic or Baltic cod population) can be caught per year to sustain the population and maximize the profits of the fishery. |
| GenomeLength | generate_genome_length_plots.py | Category for analysis of population according to different genome lengths of individuals. By default, 32, 64 and 128-bit genomes are checked. |
| LethalMutations | generate_lethal_mutations_plots.py | Presented changes between simulations when the number of lethal mutations which individual need to suffer before deaths is set to 2,3 and 4. |
| OffspringCount | generate_offspring_count_plots.py | Plots show how populations behave with different offspring count set in configuration parameters. |
| ReproductionAge | generate_reproduction_age_plots.py | Visualize differences between simulations having reproduction age set to 8, 10 and 12 years. |

All plots can be generated using the `generate_all.py` script. Make sure that you prepare 32, 64 and 128 genome length binaries first (check the [genome length](#genome-length) section for more details).

## Example basic plots
Example basic category plots generated for 50k years long simulation:
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/basic_age_distribution.png" >
    <img width="32%" src="plots/Examples/basic_bits_distribution.png" >
    <img width="32%" src="plots/Examples/basic_deaths_distribution.png" >
</p>
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/basic_population.png" >
    <img width="32%" src="plots/Examples/basic_families.png" >
    <img width="32%" src="plots/Examples/basic_births_and_deaths.png" >
</p>

## Example catching plots
Example plots for simulations with catching and 50k years duration:
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/catching_age_distribution.png" >
    <img width="32%" src="plots/Examples/catching_bits_distribution.png" >
    <img width="32%" src="plots/Examples/catching_deaths_distribution.png" >
</p>
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/catching_population.png" >
    <img width="32%" src="plots/Examples/catching_families.png" >
    <img width="32%" src="plots/Examples/catching_births_and_deaths.png" >
</p>

## Example genome length plots
Example plots for simulations with different genome lengths (32, 64, and 128 bits) and 50k years duration:
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/genome_age_distribution.png" >
    <img width="32%" src="plots/Examples/genome_bits_distribution.png" >
    <img width="32%" src="plots/Examples/genome_deaths_distribution.png" >
</p>
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/genome_population.png" >
    <img width="32%" src="plots/Examples/genome_families.png" >
    <img width="32%" src="plots/Examples/genome_births_and_deaths.png" >
</p>

## Example reproduction age plots
Examples of plots created for runs with different reproduction ages (8, 10, and 12 years) and 50k years long simulation:
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/reproduction_age_distribution.png" >
    <img width="32%" src="plots/Examples/reproduction_bits_distribution.png" >
    <img width="32%" src="plots/Examples/reproduction_deaths_distribution.png" >
</p>
<p align="center" width="100%">
    <img width="32%" src="plots/Examples/reproduction_population.png" >
    <img width="32%" src="plots/Examples/reproduction_families.png" >
    <img width="32%" src="plots/Examples/reproduction_births_and_deaths.png" >
</p>

# Licensing
Software is released under the MIT license.

The project uses the following open-source software:
| Name | License | Home | Description |
| --- | --- | --- | --- |
| yaml-cpp | MIT | https://github.com/jbeder/yaml-cpp | YAML parser library for C++ |
| Catch2 | BSL-1.0 | https://github.com/catchorg/Catch2 | testing framework for C++ |
| Argparse | MIT | https://github.com/p-ranav/argparse | an argument parser for C++ |

# Testing
For testing of code logic, there is a responsible `Catch2` framework. As it is well integrated with `CMake` it should be straightforward to use. As the first step, build the project. Make sure that the `penna-model-test` target is built. Modern IDEs supporting CMake also support running tests with monitoring of failures. But in case you would like to run it manually, go to the `tests` directory, where the⁣ binary `penna-model-tests` should be available after building. Calling it directly should produce the following output on Windows:
    
    $ penna-model-test.exe
    
    Randomness seeded to: 1752308627

    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    penna-model-test.exe is a Catch2 v3.3.0 host application.
    Run with -? for options
    
    -------------------------------------------------------------------------------
    Benchmark
    -------------------------------------------------------------------------------
    <build path>\test\SimulationTest.cpp:53
    ...............................................................................
    
    <build path>\test\SimulationTest.cpp:55: SKIPPED:
    
    ===============================================================================
    test cases:   7 |   6 passed | 1 skipped
    assertions: 111 | 111 passed

> **_NOTE:_**  Performance tests are to be skipped by default. To activate performance tests, go to `Benchmark` test case and comment `SKIP();` line.

As an alternative, CTest can be used to run tests (performance tests switched on):

    $ ctest
    Test project <build path>\test
        Start 1: Catching
    1/7 Test #1: Catching .........................   Passed    0.03 sec
        Start 2: Config
    2/7 Test #2: Config ...........................   Passed    0.02 sec
        Start 3: Output
    3/7 Test #3: Output ...........................   Passed    0.02 sec
        Start 4: Output averages
    4/7 Test #4: Output averages ..................   Passed    0.06 sec
        Start 5: Individual
    5/7 Test #5: Individual .......................   Passed    0.02 sec
        Start 6: Simulation
    6/7 Test #6: Simulation .......................   Passed    0.04 sec
        Start 7: Benchmark
    7/7 Test #7: Benchmark ........................   Passed  145.77 sec
    
    100% tests passed, 0 tests failed out of 7
    
    Total Test time (real) = 145.99 sec

# Potential further improvements
* Upgrade code to use C++20/23.
* Measure and increase test coverage.
* Setup static analysis (CppCheck, Clang Tidy, Clazy).
* Make the size of the genome configurable (use something different from std::bitset).
* Simplify the generation of results. Currently, it is somewhat complex and advanced.
