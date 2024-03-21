#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "src/Config.h"

namespace
{
bool operator==(const Config::Params& left, const Config::Params& right)
{
    return left.maxPopulation_ == right.maxPopulation_ &&
           left.years_ == right.years_ &&
           left.livesOnStart_ == right.livesOnStart_ &&
           left.mutationsAdded_ == right.mutationsAdded_ &&
           left.mutationsLethal_ == right.mutationsLethal_ &&
           left.mutationsInitial_ == right.mutationsInitial_ &&
           left.reproductionAge_ == right.reproductionAge_ &&
           left.chanceForOffspring_ == right.chanceForOffspring_ &&
           left.offspringCount_ == right.offspringCount_ &&
           left.simulationsCount_ == right.simulationsCount_;
}
}  // namespace

namespace Catch
{
template <>
struct StringMaker<Config::Params>
{
    static std::string convert(Config::Params const& value)
    {
        std::ostringstream os;
        os << "{" << value.maxPopulation_ << "," << value.years_ << ","
           << value.livesOnStart_ << "," << value.mutationsAdded_ << ","
           << value.mutationsLethal_ << "," << value.mutationsInitial_ << ","
           << value.reproductionAge_ << "," << value.chanceForOffspring_ << ","
           << value.offspringCount_ << "," << value.simulationsCount_ << "}";
        return os.str();
    }
};
}  // namespace Catch

TEST_CASE("Config", "[penna]")
{
    SECTION("empty config")
    {
        const Config::Params defaultParams;

        std::istringstream emptyCofigString("");
        const Config::Params configParams{Config::loadConfig(emptyCofigString)};
        REQUIRE(defaultParams == configParams);
    }

    SECTION("valid config")
    {
        const Config::Params expectedParams{100000, 1000, 2000, 2, 6,
                                            6,      4,    50,   2, 4};

        std::istringstream configString(R"(
maxPopulation: 100000
years: 1000
livesOnStart: 2000
mutations:
  added: 2
  lethal: 6
  initial: 6
reproductionAge: 4
offspringChance: 50
offspringCount: 2
simulations: 4)");
        const Config::Params configParams{Config::loadConfig(configString)};
        REQUIRE(configParams == expectedParams);
    }

    SECTION("invalid config")
    {
        std::istringstream invalidConfigString(R"(
mutations:
  lethal: aaaa36
)");
        REQUIRE_THROWS(Config::loadConfig(invalidConfigString));
    }

    SECTION("partial config")
    {
        Config::Params expectedParams;
        expectedParams.maxPopulation_ = 100000;
        expectedParams.simulationsCount_ = 4;

        std::istringstream configString(R"(
maxPopulation: 100000
simulations: 4)");
        const Config::Params configParams{Config::loadConfig(configString)};
        REQUIRE(configParams == expectedParams);
    }
}
