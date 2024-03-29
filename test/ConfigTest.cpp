#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "src/Config.h"

namespace
{
bool operator==(const Config::Params& left, const Config::Params& right)
{
    return left.population_.max_ == right.population_.max_ &&
           left.years_ == right.years_ &&
           left.population_.initial_ == right.population_.initial_ &&
           left.mutations_.added_ == right.mutations_.added_ &&
           left.mutations_.lethal_ == right.mutations_.lethal_ &&
           left.mutations_.initial_ == right.mutations_.initial_ &&
           left.reproductionAge_ == right.reproductionAge_ &&
           left.offspring_.chance_ == right.offspring_.chance_ &&
           left.offspring_.count_ == right.offspring_.count_ &&
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
        os << "{" << value.population_.max_ << "," << value.years_ << ","
           << value.population_.initial_ << "," << value.mutations_.added_
           << "," << value.mutations_.lethal_ << ","
           << value.mutations_.initial_ << "," << value.reproductionAge_ << ","
           << value.offspring_.chance_ << "," << value.offspring_.count_ << ","
           << value.simulationsCount_ << "}";
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
        const Config::Params expectedParams{2000, 100000, 1000, 2, 6,
                                            6,    4,      50,   2, 4};

        std::istringstream configString(R"(
population:
  initial: 2000
  max: 100000
years: 1000
mutations:
  added: 2
  lethal: 6
  initial: 6
reproductionAge: 4
offspring:
  chance: 50
  count: 2
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
        expectedParams.population_.max_ = 100000;
        expectedParams.simulationsCount_ = 4;

        std::istringstream configString(R"(
population:
  max: 100000
simulations: 4)");
        const Config::Params configParams{Config::loadConfig(configString)};
        REQUIRE(configParams == expectedParams);
    }
}
