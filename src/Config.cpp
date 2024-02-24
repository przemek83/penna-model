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
    MUTATIONS_DELTA,
    MAX_MUTATIONS,
    STARTING_MUTATIONS,
    REPRODUCTION_AGE,
    OFFSPRING_CHANCE,
    OFFSPRING_COUNT,
    SIMULATIONS
};

std::map<Field, std::string> fieldToString{
    {Field::MAX_POPULATION, "maxPopulation"},
    {Field::YEARS, "years"},
    {Field::LIVES_ON_START, "livesOnStart"},
    {Field::MUTATIONS_DELTA, "mutationsDelta"},
    {Field::MAX_MUTATIONS, "maxMutations"},
    {Field::STARTING_MUTATIONS, "startingMutations"},
    {Field::REPRODUCTION_AGE, "reproductionAge"},
    {Field::OFFSPRING_CHANCE, "offspringChance"},
    {Field::OFFSPRING_COUNT, "offspringCount"},
    {Field::SIMULATIONS, "simulations"},
};

std::string createErrorMsg(Field field, const std::string& condition)
{
    std::ostringstream os;
    os << "check failed: " << fieldToString[field] << " " << condition
       << std::endl;
    return os.str();
}
}  // namespace

namespace Config
{
Config::Params loadConfig(std::istream& configFile)
{
    Config::Params params;
    YAML::Node yaml = YAML::Load(configFile);

    if (const YAML::Node value{yaml[fieldToString[Field::MAX_POPULATION]]})
        params.maxPopulation_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::YEARS]]})
        params.years_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::LIVES_ON_START]]})
        params.livesOnStart_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::MUTATIONS_DELTA]]})
        params.mutationsDelta_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::MAX_MUTATIONS]]})
        params.maxMutations_ = value.as<int>();

    if (const YAML::Node value{yaml[fieldToString[Field::STARTING_MUTATIONS]]})
        params.startingMutations_ = value.as<int>();

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
    std::string errorString;

    if (params.maxPopulation_ <= 0)
        errorString += createErrorMsg(Field::MAX_POPULATION, "> 0");

    if (params.years_ <= 0)
        errorString += createErrorMsg(Field::YEARS, "> 0");

    if (params.livesOnStart_ <= 0)
        errorString += createErrorMsg(Field::LIVES_ON_START, "> 0");

    if (params.mutationsDelta_ < 0)
        errorString += createErrorMsg(Field::MUTATIONS_DELTA, ">= 0");

    if (params.maxMutations_ < 0)
        errorString += createErrorMsg(Field::MAX_MUTATIONS, ">= 0");

    if (params.startingMutations_ < 0)
        errorString += createErrorMsg(Field::STARTING_MUTATIONS, ">= 0");

    if (params.reproductionAge_ < 0)
        errorString += createErrorMsg(Field::REPRODUCTION_AGE, ">= 0");

    if (params.offspringCount_ < 0)
        errorString += createErrorMsg(Field::OFFSPRING_COUNT, ">= 0");

    if (params.chanceForOffspring_ < 0)
        errorString += createErrorMsg(Field::OFFSPRING_CHANCE, ">= 0");

    if (params.chanceForOffspring_ > 100)
        errorString += createErrorMsg(Field::OFFSPRING_CHANCE, "<= 100");

    if (params.simulationsCount_ < 1)
        errorString += createErrorMsg(Field::SIMULATIONS, ">= 1");

    if (params.startingMutations_ > Params::bits_)
        errorString += createErrorMsg(Field::STARTING_MUTATIONS,
                                      "<= " + std::to_string(Params::bits_));

    if (params.reproductionAge_ > Params::bits_)
        errorString += createErrorMsg(Field::REPRODUCTION_AGE,
                                      "<= " + std::to_string(Params::bits_));

    if (params.livesOnStart_ > params.maxPopulation_)
        errorString += createErrorMsg(
            Field::LIVES_ON_START,
            "lover than " + fieldToString[Field::MAX_POPULATION]);

    if (errorString.empty())
        return true;

    std::cerr << "Configuration is invalid:" << std::endl << errorString;

    return false;
}

}  // namespace Config
