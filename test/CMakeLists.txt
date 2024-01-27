SET(PROJECT_TEST ${PROJECT_NAME}-test )

include_directories(${CMAKE_SOURCE_DIR})

file(COPY TestFiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(${PROJECT_TEST}_SOURCES
    SimulationTest.cpp
    IndividualTest.cpp
    OutputTest.cpp
    MockedGenerator.h
    MockedGenerator.cpp
    Common.h
    Common.cpp
    NullOutput.h
    NullOutput.cpp
    StringOutput.h
    StringOutput.cpp
    ../Simulation.cpp
    ../Individual.cpp
    ../FileOutput.cpp
    ../Output.cpp
    ../SimulationData.cpp
    )

add_executable(${PROJECT_TEST} ${${PROJECT_TEST}_SOURCES})
target_link_libraries(${PROJECT_TEST} PRIVATE Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_TEST})
