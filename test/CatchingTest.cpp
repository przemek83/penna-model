#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

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
}  // namespace

namespace Catch
{

template <>
struct StringMaker<Config::Catching>
{
    static std::string convert(const Config::Catching& value)
    {
        std::ostringstream os;
        os << "{" << value.percent_ << "," << value.fromYear_ << ","
           << value.fromAge_ << "}";
        return os.str();
    }
};
}  // namespace Catch

TEST_CASE("Catching", "[penna]")
{
    Config::Params params;
    params.population_.max_ = 5000;
    params.years_ = 10;

    using TestCase = std::pair<Config::Catching, std::string>;
    auto [catching, expected] =
        GENERATE(TestCase{{0, 0, 4}, zeroPercentExpected},
                 TestCase{{100, 0, 4}, oneHundredPercentExpected},
                 TestCase{{50, 0, 4}, fiftyPercentExpected},
                 TestCase{{50, 0, 0}, fromAgeZeroExpected},
                 TestCase{{50, 0, 10}, zeroPercentExpected},
                 TestCase{{50, 0, 4}, fiftyPercentExpected},
                 TestCase{{50, 0, 4}, fiftyPercentExpected},
                 TestCase{{50, 10, 4}, zeroPercentExpected},
                 TestCase{{50, 5, 4}, fromYearFiveExpected});

    CAPTURE(catching);

    params.catching_ = catching;

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
