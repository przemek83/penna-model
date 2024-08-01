#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <src/Generator.h>
#include <src/Simulation.h>

#include "Common.h"
#include "StringOutput.h"

namespace
{
const std::string zeroPercentExpected{
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

const std::string oneHundredPercentExpected{
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

const std::string fiftyPercentExpected{
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

const std::string fromAgeZeroExpected{
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

const std::string fromYearFiveExpected{
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
struct StringMaker<config::Catching>
{
    static std::string convert(const config::Catching& value)
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
    config::Params params;
    params.population_.max_ = 5000;
    params.years_ = 10;

    using TestCase = std::pair<config::Catching, std::string>;
    auto [catching,
          expected]{GENERATE(TestCase{{0, 0, 4}, zeroPercentExpected},
                             TestCase{{100, 0, 4}, oneHundredPercentExpected},
                             TestCase{{50, 0, 4}, fiftyPercentExpected},
                             TestCase{{50, 0, 0}, fromAgeZeroExpected},
                             TestCase{{50, 0, 10}, zeroPercentExpected},
                             TestCase{{50, 0, 4}, fiftyPercentExpected},
                             TestCase{{50, 0, 4}, fiftyPercentExpected},
                             TestCase{{50, 10, 4}, zeroPercentExpected},
                             TestCase{{50, 5, 4}, fromYearFiveExpected})};

    CAPTURE(catching);

    params.catching_ = catching;

    Simulation simulation(params);
    simulation.setGenerator(Common::getTestGenerator(params.bits_));
    simulation.createInitialPopulation();
    const SimulationData data{simulation.run()};

    StringOutput output;
    output.saveSimulationData(data);

    const std::string current{
        output.getContentForOutputType(OutputType::STATISTICS)};
    REQUIRE(current == expected);
}
