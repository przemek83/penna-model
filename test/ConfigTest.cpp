#include <ios>
#include <iostream>
#include <memory>
#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <src/Config.h>

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

std::string getValidConfig()
{
    return R"(
population:
  initial: 2000
  max: 100000
years: 5000
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
  fromAge: 5)";
}

config::Params getExpectedParamsForValidConfig()
{
    return {2'000, 100'000, 5'000, 2, 6, 6, 4, 50, 2, 4, 20, 2'000, 5};
}

std::string getInvalidConfig()
{
    return R"(
mutations:
  lethal: aaaa36
)";
}

}  // namespace

TEST_CASE("Config loading", "[penna]")
{
    SECTION("empty config")
    {
        const config::Params defaultParams;
        auto configStream{std::make_unique<std::istringstream>("")};
        const config::Params configParams{
            config::loadConfig(std::move(configStream))};
        REQUIRE_THAT(configParams, equalsParams(defaultParams));
    }

    SECTION("valid config")
    {
        const config::Params expectedParams{getExpectedParamsForValidConfig()};
        auto configStream{
            std::make_unique<std::istringstream>(getValidConfig())};
        const config::Params configParams{
            config::loadConfig(std::move(configStream))};
        REQUIRE_THAT(configParams, equalsParams(expectedParams));
    }

    SECTION("invalid config")
    {
        std::string configString(getInvalidConfig());
        auto configStream{std::make_unique<std::istringstream>(configString)};
        REQUIRE_THROWS(config::loadConfig(std::move(configStream)));
    }

    SECTION("partial config")
    {
        config::Params expectedParams;
        expectedParams.population_.max_ = 100'000;
        expectedParams.simulationsCount_ = 4;

        std::string configString(R"(
population:
  max: 100000
simulations: 4)");
        auto configStream{std::make_unique<std::istringstream>(configString)};
        const config::Params configParams{
            config::loadConfig(std::move(configStream))};
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

    SECTION("catching percent too low")
    {
        params.catching_.percent_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("catching percent too high")
    {
        params.catching_.percent_ = config::maxPercentage + 1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("catching from year too low")
    {
        params.catching_.fromYear_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("catching from year too high")
    {
        params.catching_.fromYear_ = params.years_;
        REQUIRE(!config::isValid(params));
    }

    SECTION("catching from age too low")
    {
        params.catching_.fromAge_ = -1;
        REQUIRE(!config::isValid(params));
    }

    SECTION("catching from age too high")
    {
        params.catching_.fromAge_ = config::Params::bits_ + 1;
        REQUIRE(!config::isValid(params));
    }

    std::cerr.rdbuf(oldCerrBuffer);
}

TEST_CASE("Config app arguments", "[penna]")
{
    std::streambuf* oldCerrBuffer{std::cerr.rdbuf()};
    std::ostringstream output;
    std::cerr.rdbuf(output.rdbuf());

    const std::string defaultConfigFileName{"config.yaml"};
    const std::string defaultPrefix{"averages"};

    SECTION("no params")
    {
        const char* argv[]{"penna-model"};
        const auto [success, configFileName,
                    prefix]{config::getAppArguments(std::size(argv), argv)};
        REQUIRE(success);
        REQUIRE_THAT(configFileName,
                     Catch::Matchers::Equals(defaultConfigFileName));
        REQUIRE_THAT(prefix, Catch::Matchers::Equals(defaultPrefix));
    }

    SECTION("only config given")
    {
        std::string configName{"someConfig.yaml"};
        const char* argv[]{"penna-model", configName.c_str()};
        const auto [success, configFileName,
                    prefix]{config::getAppArguments(std::size(argv), argv)};
        REQUIRE(success);
        REQUIRE_THAT(configFileName, Catch::Matchers::Equals(configName));
        REQUIRE_THAT(prefix, Catch::Matchers::Equals(defaultPrefix));
    }

    SECTION("only prefix given")
    {
        std::string somePrefix{"somePrefix"};
        const char* argv[]{"penna-model", "-p", somePrefix.c_str()};
        const auto [success, configFileName,
                    prefix]{config::getAppArguments(std::size(argv), argv)};
        REQUIRE(success);
        REQUIRE_THAT(configFileName,
                     Catch::Matchers::Equals(defaultConfigFileName));
        REQUIRE_THAT(prefix, Catch::Matchers::Equals(somePrefix));
    }

    SECTION("config and prefix given")
    {
        std::string somePrefix{"somePrefix"};
        std::string configName{"someConfig.yaml"};
        const char* argv[]{"penna-model", configName.c_str(), "-p",
                           somePrefix.c_str()};
        const auto [success, configFileName,
                    prefix]{config::getAppArguments(std::size(argv), argv)};
        REQUIRE(success);
        REQUIRE_THAT(configFileName, Catch::Matchers::Equals(configName));
        REQUIRE_THAT(prefix, Catch::Matchers::Equals(somePrefix));
    }

    SECTION("wrong param")
    {
        const char* argv[]{"penna-model", "-wrong_param"};
        const auto [success, configFileName,
                    prefix]{config::getAppArguments(std::size(argv), argv)};
        REQUIRE(!success);
    }

    SECTION("no expected prefix")
    {
        const char* argv[]{"penna-model", "-p"};
        const auto [success, configFileName,
                    prefix]{config::getAppArguments(std::size(argv), argv)};
        REQUIRE(success);
        REQUIRE_THAT(prefix, Catch::Matchers::Equals(defaultPrefix));
    }

    std::cerr.rdbuf(oldCerrBuffer);
}

TEST_CASE("Get Params", "[penna]")
{
    SECTION("Valid config file")
    {
        const config::Params expectedParams{getExpectedParamsForValidConfig()};
        auto configStream{
            std::make_unique<std::istringstream>(getValidConfig())};
        const std::string configFileName = "config.yaml";
        const auto [success, params]{
            config::getParams(configFileName, std::move(configStream))};
        REQUIRE(success);
        REQUIRE_THAT(params, equalsParams(expectedParams));
    }

    SECTION("Invalid config file")
    {
        std::string configString(getInvalidConfig());
        auto configStream{std::make_unique<std::istringstream>(configString)};
        const std::string configFileName = "config.yaml";
        REQUIRE_THROWS(
            config::getParams(configFileName, std::move(configStream)));
    }

    SECTION("Failed to read config file")
    {
        const std::string configFileName = "nonexistent.yaml";
        auto configStream = std::make_unique<std::istringstream>();
        configStream->setstate(std::ios_base::failbit);
        const auto [success, params]{
            config::getParams(configFileName, std::move(configStream))};
        REQUIRE_FALSE(success);
    }
}