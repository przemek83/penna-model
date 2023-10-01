cmake_minimum_required(VERSION 3.8)

project(penna-model
    LANGUAGES CXX 
    VERSION 1.0.0 
    DESCRIPTION "Penna model implementation."
    HOMEPAGE_URL "https://github.com/przemek83/penna-model")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(${PROJECT_NAME}_SOURCES
    penna.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
