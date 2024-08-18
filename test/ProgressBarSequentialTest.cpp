#include <catch2/catch_test_macros.hpp>

#include <src/Config.h>
#include <src/ProgressBarSequential.h>

#include "CoutEater.h"

TEST_CASE("Sequential Progress Callback")
{
    const int years{1000};
    config::Params params;
    params.years_ = years;

    const int simId{0};
    ProgressBarSequential progressBar{params.years_, params.simulationsCount_};
    const int length{config::progressBarLength};
    const int firstYearWithMarker{(years / length) - 1};

    CoutEater coutEater;

    SECTION("start")
    {
        progressBar.update(0, simId);
        const std::string expectedOutput{
            "1/" + std::to_string(params.simulationsCount_) + " ["};
        REQUIRE(coutEater.getOutput() == expectedOutput);
    }

    SECTION("before progress")
    {
        progressBar.update(firstYearWithMarker - 1, simId);
        REQUIRE(coutEater.getOutput() == "");
    }

    SECTION("progress")
    {
        progressBar.update(firstYearWithMarker, simId);
        REQUIRE(coutEater.getOutput() == "*");
    }

    SECTION("after progress")
    {
        progressBar.update(firstYearWithMarker + 1, simId);
        REQUIRE(coutEater.getOutput() == "");
    }

    SECTION("end")
    {
        const int lastYear{years - 1};
        progressBar.update(lastYear, simId);
        REQUIRE(coutEater.getOutput() == "*]\n");
    }
}
