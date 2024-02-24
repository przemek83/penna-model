#include "Config.h"

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
}  // namespace

namespace Config
{
Config::Params loadConfig()
{
    Config::Params params;
    YAML::Node yaml = YAML::LoadFile("config.yaml");

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

}  // namespace Config
