#include <cpputils/StreamEater.h>
#include <catch2/catch_test_macros.hpp>

#include <src/Config.h>
#include <src/ProgressBarSequential.h>

TEST_CASE("Sequential Progress Callback")
{
    const int years{1000};
    config::Params params;
    params.years_ = years;

    const int simId{0};
    ProgressBarSequential progressBar{params.years_, params.simulationsCount_};
    const int length{config::progressBarLength};
    const int firstYearWithMarker{(years / length) - 1};

    StreamEater eater(std::cout);

    SECTION("start")
    {
        progressBar.update(0, simId);
        const std::string expectedOutput{
            "1/" + std::to_string(params.simulationsCount_) + " ["};
        REQUIRE(eater.getOutput() == expectedOutput);
    }

    SECTION("before progress")
    {
        progressBar.update(firstYearWithMarker - 1, simId);
        REQUIRE(eater.getOutput() == "");
    }

    SECTION("progress")
    {
        progressBar.update(firstYearWithMarker, simId);
        REQUIRE(eater.getOutput() == "*");
    }

    SECTION("after progress")
    {
        progressBar.update(firstYearWithMarker + 1, simId);
        REQUIRE(eater.getOutput() == "");
    }

    SECTION("end")
    {
        const int lastYear{years - 1};
        progressBar.update(lastYear, simId);
        REQUIRE(eater.getOutput() == "*]\n");
    }
}
