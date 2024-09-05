cmake_minimum_required(VERSION 3.14)

project(penna-model
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "Penna model implementation."
        HOMEPAGE_URL "https://github.com/przemek83/penna-model")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CODE_COVERAGE "Collect coverage" OFF)

if(CODE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage")
endif()

include(CMakeLists.txt.yaml-cpp)
include(CMakeLists.txt.argparse)
include(CMakeLists.txt.cpputils)

file(COPY config.yaml DESTINATION ${CMAKE_BINARY_DIR})
file(COPY plots DESTINATION ${CMAKE_BINARY_DIR})

set(${PROJECT_NAME}_SOURCES
    src/Output.h
    src/Output.cpp
    src/Individual.h
    src/Individual.cpp
    src/SimResults.h
    src/SimResults.cpp
    src/Simulation.h
    src/Simulation.cpp
    src/Generator.h
    src/Generator.cpp
    src/NumbersGenerator.h
    src/NumbersGenerator.cpp
    src/Config.h
    src/Config.cpp
    src/Timer.h
    src/Timer.cpp
    src/FileOutput.h
    src/FileOutput.cpp
    src/Runner.h
    src/Runner.cpp
    src/ProgressBar.h
    src/ProgressBar.cpp
    src/ProgressBarOverall.h
    src/ProgressBarOverall.cpp
    src/ProgressBarSequential.h
    src/ProgressBarSequential.cpp
    src/AvgResults.h
    src/AvgResults.cpp
    src/Results.h
    src/Results.cpp
    src/OutputType.h
    src/Logger.h
    src/Metrics.h
)

# Create default binaries.
add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}-lib yaml-cpp::yaml-cpp argparse)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

function(GENERATE_OTHER_GENOME_BIT_LENGHTS_BINARIES)
    foreach( bit_length IN ITEMS 32 64 128 )
        add_library (${PROJECT_NAME}${bit_length}-lib STATIC ${${PROJECT_NAME}_SOURCES})
        target_link_libraries(${PROJECT_NAME}${bit_length}-lib yaml-cpp::yaml-cpp argparse)
        target_compile_definitions(${PROJECT_NAME}${bit_length}-lib PRIVATE GENOME_BITS=${bit_length})
    
        add_executable(${CMAKE_PROJECT_NAME}${bit_length} src/main.cpp)
        target_link_libraries(${PROJECT_NAME}${bit_length} ${PROJECT_NAME}${bit_length}-lib)
    endforeach( bit_length IN ITEMS 32 64 128 )
endfunction()

# Uncommment to generate binaries for different genome lengths.
# generate_other_genome_bit_lenghts_binaries()

enable_testing()
Include(Tests.cmake)
add_subdirectory(test)
