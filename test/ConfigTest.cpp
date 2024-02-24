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
           left.mutationsDelta_ == right.mutationsDelta_ &&
           left.maxMutations_ == right.maxMutations_ &&
           left.startingMutations_ == right.startingMutations_ &&
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
           << value.livesOnStart_ << "," << value.mutationsDelta_ << ","
           << value.maxMutations_ << "," << value.startingMutations_ << ","
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
}
