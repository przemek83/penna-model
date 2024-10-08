#include <cpputils/StreamEater.h>
#include <catch2/catch_test_macros.hpp>

#include <src/Config.h>
#include <src/ProgressBarOverall.h>

namespace
{
void progress(ProgressBarOverall& progressBar, int steps)
{
    StreamEater eater(std::cout);
    for (int i{0}; i < steps; ++i)
        progressBar.update(0, 0);
}
}  // namespace

TEST_CASE("Overall Progress Callback")
{
    const int years{1000};
    config::Params params;
    params.simulationsCount_ = 1;
    params.years_ = years;

    ProgressBarOverall progressBar{params.years_, params.simulationsCount_};
    const int length{config::progressBarLength};
    const int firstYearWithMarker{(years / length) - 1};

    SECTION("start")
    {
        StreamEater eater(std::cout);
        progressBar.update(0, 0);
        REQUIRE(eater.getOutput() == "[");
    }

    SECTION("before progress")
    {
        progress(progressBar, firstYearWithMarker - 1);
        StreamEater eater(std::cout);
        progressBar.update(0, 0);
        REQUIRE(eater.getOutput() == "");
    }

    SECTION("progress")
    {
        progress(progressBar, firstYearWithMarker);
        StreamEater eater(std::cout);
        progressBar.update(0, 0);
        REQUIRE(eater.getOutput() == "*");
    }

    SECTION("after progress")
    {
        progress(progressBar, firstYearWithMarker + 1);
        StreamEater eater(std::cout);
        progressBar.update(0, 0);
        REQUIRE(eater.getOutput() == "");
    }

    SECTION("end")
    {
        progress(progressBar, years - 1);
        StreamEater eater(std::cout);
        progressBar.update(0, 0);
        REQUIRE(eater.getOutput() == "*]\n");
    }
}