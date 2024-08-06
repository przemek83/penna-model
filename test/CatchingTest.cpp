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
0	3000	0	1226	1774
1	1226	0	919	307
2	919	0	742	177
3	742	0	619	123
4	619	0	538	81
5	538	0	469	69
6	469	0	419	50
7	419	0	381	38
8	381	0	355	26
9	355	0	332	23
)"};

const std::string oneHundredPercentExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	1226	1774
1	1226	0	919	307
2	919	0	742	177
3	742	0	619	123
4	619	0	0	619
5	0	0	0	0
6	0	0	0	0
7	0	0	0	0
8	0	0	0	0
9	0	0	0	0
)"};

const std::string fiftyPercentExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	1226	1774
1	1226	0	919	307
2	919	0	742	177
3	742	0	619	123
4	619	0	274	345
5	274	0	128	146
6	128	0	63	65
7	63	0	27	36
8	27	0	16	11
9	16	0	7	9
)"};

const std::string fromAgeZeroExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	611	2389
1	611	0	256	355
2	256	0	121	135
3	121	0	60	61
4	60	0	26	34
5	26	0	15	11
6	15	0	8	7
7	8	0	3	5
8	3	0	2	1
9	2	0	0	2
)"};

const std::string fromYearFiveExpected{
    R"(Year	Living_start	Births	Living_end	Deaths
0	3000	0	1226	1774
1	1226	0	919	307
2	919	0	742	177
3	742	0	619	123
4	619	0	538	81
5	538	0	229	309
6	229	0	99	130
7	99	0	50	49
8	50	0	30	20
9	30	0	17	13
)"};
}  // namespace

namespace Catch
{

template <>
class StringMaker<config::Catching>
{
public:
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
    params.population_.max_ = 5'000;
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
    simulation.setGenerator(common::getTestGenerator(config::Params::bits_));
    simulation.createInitialPopulation();
    const SimulationData data{simulation.run()};

    StringOutput output;
    output.saveSimulationData(data);

    const std::string current{
        output.getContentForOutputType(OutputType::STATISTICS)};
    REQUIRE(current == expected);
}
