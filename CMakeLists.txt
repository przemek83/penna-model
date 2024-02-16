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

file(COPY config.yaml DESTINATION ${CMAKE_BINARY_DIR})

set(${PROJECT_NAME}_SOURCES
    src/main.cpp
    src/Output.h
    src/Output.cpp
    src/Individual.h
    src/Individual.cpp
    src/Common.h
    src/SimulationData.h
    src/Simulation.h
    src/Simulation.cpp
    src/Generator.h
    src/Generator.cpp
    src/NumbersGenerator.h
    src/NumbersGenerator.cpp
    src/Config.h
    src/Timer.h
    src/Timer.cpp
    src/FileOutput.h
    src/FileOutput.cpp
)

add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}-lib yaml-cpp::yaml-cpp)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

enable_testing()
Include(Tests.cmake)
add_subdirectory(test)
