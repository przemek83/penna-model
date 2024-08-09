#include <iostream>
#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <src/Config.h>
#include <src/ProgressBarOverall.h>

TEST_CASE("Overall Progress Callback")
{
    const int years{1000};
    config::Params params;
    params.simulationsCount_ = 1;
    params.years_ = years;

    const int simId{0};
    ProgressBarOverall progressBar{params.years_, params.simulationsCount_};
    const int length{config::progressBarLength};
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