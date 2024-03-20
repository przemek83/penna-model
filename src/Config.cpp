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
    MUTATIONS_ADDED_WITH_BIRTH,
    MUTATIONS_DEATH_AFTER,
    MUTATIONS_STARTING,
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
    {Field::MUTATIONS_ADDED_WITH_BIRTH, "addedWithBirth"},
    {Field::MUTATIONS_DEATH_AFTER, "deathAfter"},
    {Field::MUTATIONS_STARTING, "starting"},
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

    if (const YAML::Node value{
            node[fieldToString[Field::MUTATIONS_ADDED_WITH_BIRTH]]})
        params.mutationsDelta_ = value.as<int>();

    if (const YAML::Node value{
            node[fieldToString[Field::MUTATIONS_DEATH_AFTER]]})
        params.maxMutations_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::MUTATIONS_STARTING]]})
        params.startingMutations_ = value.as<int>();
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

    if (params.mutationsDelta_ < 0)
        errorMsg += createErrorMsg(Field::MUTATIONS_ADDED_WITH_BIRTH, ">= 0",
                                   params.mutationsDelta_);

    if (params.maxMutations_ < 0)
        errorMsg += createErrorMsg(Field::MUTATIONS_DEATH_AFTER, ">= 0",
                                   params.maxMutations_);

    if (params.startingMutations_ < 0)
        errorMsg += createErrorMsg(Field::MUTATIONS_STARTING, ">= 0",
                                   params.startingMutations_);

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

    if (params.startingMutations_ > Params::bits_)
        errorMsg += createErrorMsg(Field::MUTATIONS_STARTING,
                                   "<= " + std::to_string(Params::bits_),
                                   params.startingMutations_);

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
