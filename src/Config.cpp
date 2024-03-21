#include "Config.h"
#include <iostream>

#include "yaml-cpp/yaml.h"

namespace
{
enum class Field
{
    MAX_POPULATION,
    YEARS,
    LIVES_ON_START,
    MUTATIONS,
    MUTATIONS_ADDED,
    MUTATIONS_LETHAL,
    MUTATIONS_INITIAL,
    REPRODUCTION_AGE,
    OFFSPRING_CHANCE,
    OFFSPRING_COUNT,
    SIMULATIONS
};

std::map<Field, std::string> fieldToString{
    {Field::MAX_POPULATION, "maxPopulation"},
    {Field::YEARS, "years"},
    {Field::LIVES_ON_START, "livesOnStart"},
    {Field::MUTATIONS, "mutations"},
    {Field::MUTATIONS_ADDED, "added"},
    {Field::MUTATIONS_LETHAL, "lethal"},
    {Field::MUTATIONS_INITIAL, "initial"},
    {Field::REPRODUCTION_AGE, "reproductionAge"},
    {Field::OFFSPRING_CHANCE, "offspringChance"},
    {Field::OFFSPRING_COUNT, "offspringCount"},
    {Field::SIMULATIONS, "simulations"},
};

std::string createErrorMsg(Field field, const std::string& condition,
                           int currentValue)
{
    std::ostringstream os;
    os << "check failed: " << fieldToString[field] << " " << condition
       << ", got " << currentValue << std::endl;
    return os.str();
}

void loadMutations(const YAML::Node& yaml, Config::Params& params)
{
    const YAML::Node& node{yaml[fieldToString[Field::MUTATIONS]]};

    if (!node)
        return;

    if (const YAML::Node value{node[fieldToString[Field::MUTATIONS_ADDED]]})
        params.mutations_.added_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::MUTATIONS_LETHAL]]})
        params.mutations_.lethal_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::MUTATIONS_INITIAL]]})
        params.mutations_.initial_ = value.as<int>();
}
}  // namespace

namespace Config
{
Config::Params loadConfig(std::istream& configFile)
{
    Config::Params params;
    YAML::Node yaml{YAML::Load(configFile)};

    if (const YAML::Node value{yaml[fieldToString[Field::MAX_POPULATION]]})
        params.maxPopulation_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::YEARS]]})
        params.years_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::LIVES_ON_START]]})
        params.livesOnStart_ = value.as<int>();

    loadMutations(yaml, params);

    if (const YAML::Node value{yaml[fieldToString[Field::REPRODUCTION_AGE]]})
        params.reproductionAge_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::OFFSPRING_CHANCE]]})
        params.chanceForOffspring_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::OFFSPRING_COUNT]]})
        params.offspringCount_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::SIMULATIONS]]})
        params.simulationsCount_ = value.as<int>();

    return params;
}

bool isValid(const Params& params)
{
    std::string errorMsg;

    if (params.maxPopulation_ <= 0)
        errorMsg +=
            createErrorMsg(Field::MAX_POPULATION, "> 0", params.maxPopulation_);

    if (params.years_ <= 0)
        errorMsg += createErrorMsg(Field::YEARS, "> 0", params.years_);

    if (params.livesOnStart_ <= 0)
        errorMsg +=
            createErrorMsg(Field::LIVES_ON_START, "> 0", params.livesOnStart_);

    if (params.mutations_.added_ < 0)
        errorMsg += createErrorMsg(Field::MUTATIONS_ADDED, ">= 0",
                                   params.mutations_.added_);

    if (params.mutations_.lethal_ < 0)
        errorMsg += createErrorMsg(Field::MUTATIONS_LETHAL, ">= 0",
                                   params.mutations_.lethal_);

    if (params.mutations_.initial_ < 0)
        errorMsg += createErrorMsg(Field::MUTATIONS_INITIAL, ">= 0",
                                   params.mutations_.initial_);

    if (params.reproductionAge_ < 0)
        errorMsg += createErrorMsg(Field::REPRODUCTION_AGE, ">= 0",
                                   params.reproductionAge_);

    if (params.offspringCount_ < 0)
        errorMsg += createErrorMsg(Field::OFFSPRING_COUNT, ">= 0",
                                   params.offspringCount_);

    if (params.chanceForOffspring_ < 0)
        errorMsg += createErrorMsg(Field::OFFSPRING_CHANCE, ">= 0",
                                   params.chanceForOffspring_);

    if (params.chanceForOffspring_ > 100)
        errorMsg += createErrorMsg(Field::OFFSPRING_CHANCE, "<= 100",
                                   params.chanceForOffspring_);

    if (params.simulationsCount_ < 1)
        errorMsg += createErrorMsg(Field::SIMULATIONS, ">= 1",
                                   params.simulationsCount_);

    if (params.mutations_.initial_ > Params::bits_)
        errorMsg += createErrorMsg(Field::MUTATIONS_INITIAL,
                                   "<= " + std::to_string(Params::bits_),
                                   params.mutations_.initial_);

    if (params.reproductionAge_ > Params::bits_)
        errorMsg += createErrorMsg(Field::REPRODUCTION_AGE,
                                   "<= " + std::to_string(Params::bits_),
                                   params.reproductionAge_);

    if (params.livesOnStart_ > params.maxPopulation_)
        errorMsg +=
            createErrorMsg(Field::LIVES_ON_START,
                           "lover than " + fieldToString[Field::MAX_POPULATION],
                           params.livesOnStart_);

    if (errorMsg.empty())
        return true;

    std::cerr << "Configuration is invalid:" << std::endl << errorMsg;

    return false;
}

}  // namespace Config
