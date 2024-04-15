## Table of content
- [About project](#about-project)
- [Building](#building)
- [Usage](#usage)
- [Licensing](#licensing)
- [Testing](#testing)
- [Potential further improvements](#potential-further-improvements)


# About project
[WIP] Implementation of Penna population aging model. TODO
  
# Building
Clone and use CMake directly or via any CMake supporting IDE. CMake **should**:
+ configure everything automatically,
+ compile and create binaries.

As a result of compilation binary for simulations and binary for testing should be created.

## Used tools and libs
| Tool |  Windows | Lubuntu |
| --- | --- | --- |
| OS version | 10 22H2 | 22.04 |
| GCC | 11.2.0 | 11.3.0 |
| CMake | 3.25.0 | 3.25.0 |
| Git | 2.38.1 | 2.34.1 |
| Qt Creator | 10.0.2 | 10.0.2 |

# Usage
TODO

## Licensing
Software is licenced under MIT licence.

The project uses the following software and licenses:    
* yaml-cpp TODO,    
* Catch2 TODO,
* Argparse TODO.

## Testing
TODO

## Potential further improvements
* Upgrade code to use C++20/23.
* Measure and increase test coverage.
* Setup static analysis (CppCheck, Clang Tidy, Clazy).
* Make size of genome configurable (use something different than std::bitset).


TODO