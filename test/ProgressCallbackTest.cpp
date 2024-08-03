#include <iostream>
#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <src/Config.h>
#include <src/ProgressCallback.h>

TEST_CASE("Sequential Progress Callback")
{
    const int years{1000};
    config::Params params;
    params.years_ = years;

    const int simNumber{1};
    const int progressLineLength{progress_callback::getLineLength()};
    auto callback{progress_callback::getSequentialProgressCallback};

    std::streambuf* oldCoutBuffer{std::cout.rdbuf()};
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());
    output.str("");

    SECTION("start")
    {
        callback(simNumber, params)(0);
        const std::string expectedOutput{
            "1/" + std::to_string(params.simulationsCount_) + " ["};
        REQUIRE(output.str() == expectedOutput);
    }

    SECTION("before progress")
    {
        callback(simNumber, params)(years / progressLineLength - 1);
        REQUIRE(output.str() == "");
    }

    SECTION("progress")
    {
        callback(simNumber, params)(years / progressLineLength);
        REQUIRE(output.str() == "*");
    }

    SECTION("after progress")
    {
        callback(simNumber, params)(years / progressLineLength + 1);
        REQUIRE(output.str() == "");
    }

    SECTION("end")
    {
        callback(simNumber, params)(years);
        REQUIRE(output.str() == "*]\n");
    }

    std::cout.rdbuf(oldCoutBuffer);
}
