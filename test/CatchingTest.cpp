#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include <src/Simulation.h>

#include "MockedGenerator.h"
#include "StringOutput.h"

namespace
{
std::string zeroPercentExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	1198	1802
1	1198	0	901	297
2	901	0	721	180
3	721	0	613	108
4	613	0	536	77
5	536	0	472	64
6	472	0	414	58
7	414	0	374	40
8	374	0	354	20
9	354	0	328	26
)"};

std::string oneHundredPercentExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	1198	1802
1	1198	0	901	297
2	901	0	721	180
3	721	0	613	108
4	613	0	0	613
5	0	0	0	0
6	0	0	0	0
7	0	0	0	0
8	0	0	0	0
9	0	0	0	0
)"};

std::string fiftyPercentExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	1198	1802
1	1198	0	901	297
2	901	0	721	180
3	721	0	613	108
4	613	0	256	357
5	256	0	135	121
6	135	0	55	80
7	55	0	21	34
8	21	0	10	11
9	10	0	6	4
)"};

std::string fromAgeZeroExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	582	2418
1	582	0	234	348
2	234	0	103	131
3	103	0	50	53
4	50	0	22	28
5	22	0	11	11
6	11	0	6	5
7	6	0	4	2
8	4	0	1	3
9	1	0	0	1
)"};

std::string fromYearFiveExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	1198	1802
1	1198	0	901	297
2	901	0	721	180
3	721	0	613	108
4	613	0	536	77
5	536	0	235	301
6	235	0	99	136
7	99	0	55	44
8	55	0	25	30
9	25	0	14	11
)"};

void checkSimOutput(Config::Params params, std::string expected)
{
    StringOutput output(params.years_);

    Simulation simulation(params);

    auto generator{std::make_shared<MockedGenerator>()};
    simulation.setGenerator(generator);
    simulation.createInitialPopulation();
    const SingleSimulationData data{simulation.run()};
    output.saveSimulationData(data);

    std::string current{output.getContentForOutputType(Output::STATISTICS)};
    REQUIRE(current == expected);
}
}  // namespace

TEST_CASE("Catching", "[penna]")
{
    auto generator{std::make_shared<MockedGenerator>()};
    Config::Params params;
    params.population_.max_ = 5000;
    params.years_ = 10;
    params.catching_.fromYear_ = 0;
    params.catching_.fromAge_ = 4;
    params.catching_.percent_ = 50;

    SECTION("0 percent")
    {
        params.catching_.percent_ = 0;
        checkSimOutput(params, zeroPercentExpected);
    }

    SECTION("100 percent")
    {
        params.catching_.percent_ = 100;
        checkSimOutput(params, oneHundredPercentExpected);
    }

    SECTION("50 percent")
    {
        params.catching_.percent_ = 50;
        checkSimOutput(params, fiftyPercentExpected);
    }

    SECTION("from age 0")
    {
        params.catching_.fromAge_ = 0;
        checkSimOutput(params, fromAgeZeroExpected);
    }

    SECTION("from age 10")
    {
        params.catching_.fromAge_ = 10;
        checkSimOutput(params, zeroPercentExpected);
    }

    SECTION("from age 4")
    {
        params.catching_.fromAge_ = 4;
        checkSimOutput(params, fiftyPercentExpected);
    }

    SECTION("from year 0")
    {
        params.catching_.fromYear_ = 0;
        checkSimOutput(params, fiftyPercentExpected);
    }

    SECTION("from year 10")
    {
        params.catching_.fromYear_ = 10;
        checkSimOutput(params, zeroPercentExpected);
    }

    SECTION("from year 5")
    {
        params.catching_.fromYear_ = 5;
        checkSimOutput(params, fromYearFiveExpected);
    }
}
