cmake_minimum_required(VERSION 3.14)

project(penna-model
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "Penna model implementation."
        HOMEPAGE_URL "https://github.com/przemek83/penna-model")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CMakeLists.txt.yaml-cpp)
include(CMakeLists.txt.argparse)

file(COPY config.yaml DESTINATION ${CMAKE_BINARY_DIR})
file(COPY plots DESTINATION ${CMAKE_BINARY_DIR})

set(${PROJECT_NAME}_SOURCES
    src/Output.h
    src/Output.cpp
    src/Individual.h
    src/Individual.cpp
    src/SimulationData.h
    src/SimulationData.cpp
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
    src/ProgressCallback.h
    src/ProgressCallback.cpp
    src/AverageData.h
    src/AverageData.cpp
    src/ResultsData.h
    src/ResultsData.cpp
)

add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}-lib yaml-cpp::yaml-cpp argparse)

add_library (${PROJECT_NAME}32-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}32-lib yaml-cpp::yaml-cpp argparse)
target_compile_definitions(${PROJECT_NAME}32-lib PUBLIC GENOME_BITS=32)

add_library (${PROJECT_NAME}64-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}64-lib yaml-cpp::yaml-cpp argparse)
target_compile_definitions(${PROJECT_NAME}64-lib PUBLIC GENOME_BITS=64)

add_library (${PROJECT_NAME}128-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}128-lib yaml-cpp::yaml-cpp argparse)
target_compile_definitions(${PROJECT_NAME}128-lib PUBLIC GENOME_BITS=128)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

add_executable(${CMAKE_PROJECT_NAME}32 src/main.cpp)
target_link_libraries(${PROJECT_NAME}32 ${PROJECT_NAME}32-lib)

add_executable(${PROJECT_NAME}64 src/main.cpp)
target_link_libraries(${PROJECT_NAME}64  ${PROJECT_NAME}64-lib)

add_executable(${PROJECT_NAME}128 src/main.cpp)
target_link_libraries(${PROJECT_NAME}128  ${PROJECT_NAME}128-lib)

enable_testing()
Include(Tests.cmake)
add_subdirectory(test)
