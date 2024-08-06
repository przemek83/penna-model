#include <iostream>
#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <catch2/matchers/catch_matchers.hpp>
#include "src/Config.h"

namespace Catch
{

template <>
class StringMaker<config::Params>
{
public:
    static std::string convert(const config::Params& value)
    {
        std::ostringstream os;
        os << "{" << value.population_.max_ << "," << value.years_ << ","
           << value.population_.initial_ << "," << value.mutations_.added_
           << "," << value.mutations_.lethal_ << ","
           << value.mutations_.initial_ << "," << value.reproductionAge_ << ","
           << value.offspring_.chance_ << "," << value.offspring_.count_ << ","
           << value.simulationsCount_ << "," << value.catching_.percent_ << ","
           << value.catching_.fromYear_ << "," << value.catching_.fromAge_
           << "}";
        return os.str();
    }
};
}  // namespace Catch

namespace
{
bool operator==(const config::Catching& left, const config::Catching& right)
{
    return left.percent_ == right.percent_ &&
           left.fromYear_ == right.fromYear_ && left.fromAge_ == right.fromAge_;
}

bool operator==(const config::Population& left, const config::Population& right)
{
    return left.max_ == right.max_ && left.initial_ == right.initial_;
}

bool operator==(const config::Mutations& left, const config::Mutations& right)
{
    return left.added_ == right.added_ && left.lethal_ == right.lethal_ &&
           left.initial_ == right.initial_;
}

bool operator==(const config::Offspring& left, const config::Offspring& right)
{
    return left.chance_ == right.chance_ && left.count_ == right.count_;
}

bool operator==(const config::Params& left, const config::Params& right)
{
    return left.population_ == right.population_ &&
           left.years_ == right.years_ && left.mutations_ == right.mutations_ &&
           left.reproductionAge_ == right.reproductionAge_ &&
           left.offspring_ == right.offspring_ &&
           left.simulationsCount_ == right.simulationsCount_ &&
           left.catching_ == right.catching_;
}

class ParamsMatcher : public Catch::Matchers::MatcherBase<config::Params>
{
public:
    explicit ParamsMatcher(const config::Params& expected) : expected_(expected)
    {
    }

    bool match(const config::Params& arg) const override
    {
        return arg == expected_;
    }

    std::string describe() const override
    {
        return Catch::StringMaker<config::Params>::convert(expected_);
    }

private:
    config::Params expected_;
};

inline ParamsMatcher equalsParams(const config::Params& expected)
{
    return ParamsMatcher(expected);
}
}  // namespace

TEST_CASE("Config loading", "[penna]")
{
    SECTION("empty config")
    {
        const config::Params defaultParams;

        std::istringstream emptyCofigString("");
        const config::Params configParams{config::loadConfig(emptyCofigString)};
        REQUIRE_THAT(configParams, equalsParams(defaultParams));
    }

    SECTION("valid config")
    {
        const config::Params expectedParams{
            2'000, 100'000, 1'000, 2, 6, 6, 4, 50, 2, 4, 20, 2'000, 5};

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
simulations: 4
catching:
  percent: 20
  fromYear: 2000
  fromAge: 5)");
        const config::Params configParams{config::loadConfig(configString)};
        REQUIRE_THAT(configParams, equalsParams(expectedParams));
    }

    SECTION("invalid config")
    {
        std::istringstream invalidConfigString(R"(
mutations:
  lethal: aaaa36
)");
        REQUIRE_THROWS(config::loadConfig(invalidConfigString));
    }

    SECTION("partial config")
    {
        config::Params expectedParams;
        expectedParams.population_.max_ = 100'000;
        expectedParams.simulationsCount_ = 4;

        std::istringstream configString(R"(
population:
  max: 100000
simulations: 4)");
        const config::Params configParams{config::loadConfig(configString)};
        REQUIRE_THAT(configParams, equalsParams(expectedParams));
    }
}

TEST_CASE("Config correctness", "[penna]")
{
    config::Params params;
    std::streambuf* oldCerrBuffer{std::cerr.rdbuf()};
    std::ostringstream output;
    std::cerr.rdbuf(output.rdbuf());

    SECTION("correct params") { REQUIRE(config::isValid(params)); }

    SECTION("years incorrect")
    {
        params.years_ = 0;
        REQUIRE(!config::isValid(params));
    }

    SECTION("reproduction age too low")
    {
        params.reproductionAge_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("reproduction age too high")
    {
        params.reproductionAge_ = config::Params::bits_ + 1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("simulations count incorrect")
    {
        params.simulationsCount_ = 0;
        REQUIRE(!config::isValid(params));
    }

    SECTION("population max incorrect")
    {
        params.population_.max_ = 0;
        REQUIRE(!config::isValid(params));
    }

    SECTION("population initial too low")
    {
        params.population_.initial_ = 0;
        REQUIRE(!config::isValid(params));
    }

    SECTION("population initial too high")
    {
        params.population_.initial_ = params.population_.max_ + 1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("mutations added incorrect")
    {
        params.mutations_.added_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("mutations lethal incorrect")
    {
        params.mutations_.lethal_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("mutations initial too low")
    {
        params.mutations_.initial_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("mutations initial too high")
    {
        params.mutations_.initial_ = config::Params::bits_ + 1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("offspring count incorrect")
    {
        params.offspring_.count_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("offspring chance too low")
    {
        params.offspring_.chance_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("offspring chance too high")
    {
        params.offspring_.chance_ = config::maxPercentage + 1;
        REQUIRE(!config::isValid(params));
    }

    std::cerr.rdbuf(oldCerrBuffer);
}
