#include <iostream>
#include <memory>
#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <src/Config.h>
#include <src/ProgressBarOverall.h>
#include <src/ProgressBarSequential.h>

TEST_CASE("Sequential Progress Callback")
{
    const int years{1000};
    config::Params params;
    params.years_ = years;

    const int simId{0};
    ProgressBarSequential progressBar{params.years_, params.simulationsCount_};
    const int length{progressBar.getLength()};
    const int firstYearWithMarker{(years / length) - 1};

    std::streambuf* oldCoutBuffer{std::cout.rdbuf()};
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());
    output.str("");

    SECTION("start")
    {
        progressBar.update(0, simId);
        const std::string expectedOutput{
            "1/" + std::to_string(params.simulationsCount_) + " ["};
        REQUIRE(output.str() == expectedOutput);
    }

    SECTION("before progress")
    {
        progressBar.update(firstYearWithMarker - 1, simId);
        REQUIRE(output.str() == "");
    }

    SECTION("progress")
    {
        progressBar.update(firstYearWithMarker, simId);
        REQUIRE(output.str() == "*");
    }

    SECTION("after progress")
    {
        progressBar.update(firstYearWithMarker + 1, simId);
        REQUIRE(output.str() == "");
    }

    SECTION("end")
    {
        const int lastYear{years - 1};
        progressBar.update(lastYear, simId);
        REQUIRE(output.str() == "*]\n");
    }

    std::cout.rdbuf(oldCoutBuffer);
}

TEST_CASE("Overall Progress Callback")
{
    const int years{1000};
    config::Params params;
    params.simulationsCount_ = 1;
    params.years_ = years;

    const int simId{0};
    ProgressBarOverall progressBar{params.years_, params.simulationsCount_};
    const int length{progressBar.getLength()};
    const int firstYearWithMarker{(years / length) - 1};

    std::streambuf* oldCoutBuffer{std::cout.rdbuf()};
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());
    output.str("");

    SECTION("start")
    {
        progressBar.update(0, simId);
        REQUIRE(output.str() == "[");
    }

    SECTION("before progress")
    {
        progressBar.update(firstYearWithMarker - 1, simId);
        REQUIRE(output.str() == "");
    }

    SECTION("progress")
    {
        progressBar.update(firstYearWithMarker, simId);
        REQUIRE(output.str() == "*");
    }

    SECTION("after progress")
    {
        progressBar.update(firstYearWithMarker + 1, simId);
        REQUIRE(output.str() == "");
    }

    SECTION("end")
    {
        const int lastYear{years - 1};
        progressBar.update(lastYear, simId);
        REQUIRE(output.str() == "*]\n");
    }

    std::cout.rdbuf(oldCoutBuffer);
}