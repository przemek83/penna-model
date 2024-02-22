#include "Config.h"

#include "yaml-cpp/yaml.h"

namespace
{
enum class Fields
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

std::map<Fields, std::string> fieldToString{
    {Fields::MAX_POPULATION, "maxPopulation"},
    {Fields::YEARS, "years"},
    {Fields::LIVES_ON_START, "livesOnStart"},
    {Fields::MUTATIONS_DELTA, "mutationsDelta"},
    {Fields::MAX_MUTATIONS, "maxMutations"},
    {Fields::STARTING_MUTATIONS, "startingMutations"},
    {Fields::REPRODUCTION_AGE, "reproductionAge"},
    {Fields::OFFSPRING_CHANCE, "offspringChance"},
    {Fields::OFFSPRING_COUNT, "offspringCount"},
    {Fields::SIMULATIONS, "simulations"},
};
}  // namespace

namespace Config
{
Config::Params loadConfig()
{
    Config::Params params;
    YAML::Node yaml = YAML::LoadFile("config.yaml");
    params.years_ = yaml[fieldToString[Fields::YEARS]].as<int>();
    params.livesOnStart_ =
        yaml[fieldToString[Fields::LIVES_ON_START]].as<int>();

    return params;
}

}  // namespace Config
