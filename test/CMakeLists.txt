SET(PROJECT_TEST ${PROJECT_NAME}-test )

include_directories(${CMAKE_SOURCE_DIR})

file(COPY TestFiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(${PROJECT_TEST}_SOURCES
    SimulationTest.cpp
    IndividualTest.cpp
    OutputTest.cpp
    ConfigTest.cpp
    MockedGenerator.h
    MockedGenerator.cpp
    Common.h
    Common.cpp
    StringOutput.h
    StringOutput.cpp
    CatchingTest.cpp
    NumbersGeneratorTest.cpp
    FakeGenerator.h
    FakeGenerator.cpp
    ProgressBarSequentialTest.cpp
    ProgressBarOverallTest.cpp
    RunnerTest.cpp
    StdStreamEater.h
    StdStreamEater.cpp
    MetricsTest.cpp
    )

add_executable(${PROJECT_TEST} ${${PROJECT_TEST}_SOURCES})
target_link_libraries(${PROJECT_TEST} PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME}-lib)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_TEST})
