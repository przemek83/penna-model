#include "Config.h"
#include <iostream>

#include "yaml-cpp/yaml.h"

namespace
{
enum class Field
{
    POPULATION,
    POPULATION_INITIAL,
    POPULATION_MAX,
    YEARS,
    MUTATIONS,
    MUTATIONS_ADDED,
    MUTATIONS_LETHAL,
    MUTATIONS_INITIAL,
    REPRODUCTION_AGE,
    OFFSPRING,
    OFFSPRING_CHANCE,
    OFFSPRING_COUNT,
    SIMULATIONS,
    CATCHING,
    CATCHING_PERCENT,
    CATCHING_FROM_YEAR,
    CATCHING_FROM_AGE
};

std::map<Field, std::string> fieldToString{
    {Field::POPULATION, "population"},
    {Field::POPULATION_INITIAL, "initial"},
    {Field::POPULATION_MAX, "max"},
    {Field::YEARS, "years"},
    {Field::MUTATIONS, "mutations"},
    {Field::MUTATIONS_ADDED, "added"},
    {Field::MUTATIONS_LETHAL, "lethal"},
    {Field::MUTATIONS_INITIAL, "initial"},
    {Field::REPRODUCTION_AGE, "reproductionAge"},
    {Field::OFFSPRING, "offspring"},
    {Field::OFFSPRING_CHANCE, "chance"},
    {Field::OFFSPRING_COUNT, "count"},
    {Field::SIMULATIONS, "simulations"},
    {Field::CATCHING, "catching"},
    {Field::CATCHING_PERCENT, "percent"},
    {Field::CATCHING_FROM_YEAR, "fromYear"},
    {Field::CATCHING_FROM_AGE, "fromAge"}};

std::string createErrorMsg(Field field, const std::string& condition,
                           int currentValue)
{
    std::ostringstream os;
    os << "check failed: " << fieldToString[field] << " " << condition
       << ", got " << currentValue << std::endl;
    return os.str();
}

Config::Mutations loadMutations(const YAML::Node& node)
{
    Config::Mutations mutations;

    if (const YAML::Node value{node[fieldToString[Field::MUTATIONS_ADDED]]})
        mutations.added_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::MUTATIONS_LETHAL]]})
        mutations.lethal_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::MUTATIONS_INITIAL]]})
        mutations.initial_ = value.as<int>();

    return mutations;
}

Config::Offspring loadOffspring(const YAML::Node& node)
{
    Config::Offspring offspring;

    if (const YAML::Node value{node[fieldToString[Field::OFFSPRING_CHANCE]]})
        offspring.chance_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::OFFSPRING_COUNT]]})
        offspring.count_ = value.as<int>();

    return offspring;
}

Config::Population loadPopulation(const YAML::Node& node)
{
    Config::Population population;

    if (const YAML::Node value{node[fieldToString[Field::POPULATION_INITIAL]]})
        population.initial_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::POPULATION_MAX]]})
        population.max_ = value.as<int>();

    return population;
}

Config::Catching loadCatching(const YAML::Node& node)
{
    Config::Catching catching;

    if (const YAML::Node value{node[fieldToString[Field::CATCHING_PERCENT]]})
        catching.percent_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::CATCHING_FROM_YEAR]]})
        catching.fromYear_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString[Field::CATCHING_FROM_AGE]]})
        catching.fromAge_ = value.as<int>();

    return catching;
}

void checkMutations(Config::Mutations mutations, std::string& errorMsg)
{
    if (mutations.added_ < 0)
        errorMsg +=
            createErrorMsg(Field::MUTATIONS_ADDED, ">= 0", mutations.added_);

    if (mutations.lethal_ < 0)
        errorMsg +=
            createErrorMsg(Field::MUTATIONS_LETHAL, ">= 0", mutations.lethal_);

    if (mutations.initial_ < 0)
        errorMsg += createErrorMsg(Field::MUTATIONS_INITIAL, ">= 0",
                                   mutations.initial_);

    if (mutations.initial_ > Config::Params::bits_)
        errorMsg += createErrorMsg(
            Field::MUTATIONS_INITIAL,
            "<= " + std::to_string(Config::Params::bits_), mutations.initial_);
}

void checkOffspring(Config::Offspring offspring, std::string& errorMsg)
{
    if (offspring.count_ < 0)
        errorMsg +=
            createErrorMsg(Field::OFFSPRING_COUNT, ">= 0", offspring.count_);

    if (offspring.chance_ < 0)
        errorMsg +=
            createErrorMsg(Field::OFFSPRING_CHANCE, ">= 0", offspring.chance_);

    if (offspring.chance_ > 100)
        errorMsg += createErrorMsg(Field::OFFSPRING_CHANCE, "<= 100",
                                   offspring.chance_);
}

void checkPopulation(Config::Population population, std::string& errorMsg)
{
    if (population.max_ <= 0)
        errorMsg +=
            createErrorMsg(Field::POPULATION_MAX, "> 0", population.max_);

    if (population.initial_ <= 0)
        errorMsg += createErrorMsg(Field::POPULATION_INITIAL, "> 0",
                                   population.initial_);

    if (population.initial_ > population.max_)
        errorMsg +=
            createErrorMsg(Field::POPULATION_INITIAL,
                           "lover than " + fieldToString[Field::POPULATION_MAX],
                           population.initial_);
}

void checkCatching(Config::Catching catching, int years, std::string& errorMsg)
{
    if (catching.percent_ < 0)
        errorMsg +=
            createErrorMsg(Field::CATCHING_PERCENT, ">= 0", catching.percent_);

    if (catching.percent_ > 100)
        errorMsg += createErrorMsg(Field::CATCHING_PERCENT, "<= 100",
                                   catching.percent_);

    if (catching.fromYear_ < 0)
        errorMsg += createErrorMsg(Field::CATCHING_FROM_YEAR, ">= 0",
                                   catching.fromYear_);

    if (catching.fromYear_ >= years)
        errorMsg +=
            createErrorMsg(Field::CATCHING_FROM_YEAR,
                           "< " + std::to_string(years), catching.fromYear_);

    if (catching.fromAge_ < 0)
        errorMsg +=
            createErrorMsg(Field::CATCHING_FROM_AGE, ">= 0", catching.fromAge_);

    if (catching.fromAge_ > Config::Params::bits_)
        errorMsg += createErrorMsg(Field::CATCHING_FROM_AGE,
                                   "< " + std::to_string(Config::Params::bits_),
                                   catching.fromAge_);
}

}  // namespace

namespace Config
{
Config::Params loadConfig(std::istream& configFile)
{
    Config::Params params;
    YAML::Node yaml{YAML::Load(configFile)};

    if (const YAML::Node& node{yaml[fieldToString[Field::POPULATION]]}; node)
        params.population_ = loadPopulation(node);

    if (const YAML::Node value{yaml[fieldToString[Field::YEARS]]})
        params.years_ = value.as<int>();

    if (const YAML::Node& node{yaml[fieldToString[Field::MUTATIONS]]}; node)
        params.mutations_ = loadMutations(node);

    if (const YAML::Node value{yaml[fieldToString[Field::REPRODUCTION_AGE]]})
        params.reproductionAge_ = value.as<int>();

    if (const YAML::Node& node{yaml[fieldToString[Field::OFFSPRING]]}; node)
        params.offspring_ = loadOffspring(node);

    if (const YAML::Node value{yaml[fieldToString[Field::SIMULATIONS]]})
        params.simulationsCount_ = value.as<int>();

    if (const YAML::Node& node{yaml[fieldToString[Field::CATCHING]]}; node)
        params.catching_ = loadCatching(node);

    return params;
}

bool isValid(const Params& params)
{
    std::string errorMsg;

    if (params.years_ <= 0)
        errorMsg += createErrorMsg(Field::YEARS, "> 0", params.years_);

    if (params.reproductionAge_ < 0)
        errorMsg += createErrorMsg(Field::REPRODUCTION_AGE, ">= 0",
                                   params.reproductionAge_);

    if (params.simulationsCount_ < 1)
        errorMsg += createErrorMsg(Field::SIMULATIONS, ">= 1",
                                   params.simulationsCount_);

    if (params.reproductionAge_ > Params::bits_)
        errorMsg += createErrorMsg(Field::REPRODUCTION_AGE,
                                   "<= " + std::to_string(Params::bits_),
                                   params.reproductionAge_);

    checkPopulation(params.population_, errorMsg);

    checkMutations(params.mutations_, errorMsg);

    checkOffspring(params.offspring_, errorMsg);

    checkCatching(params.catching_, params.years_, errorMsg);

    if (errorMsg.empty())
        return true;

    std::cerr << "Configuration is invalid:" << std::endl << errorMsg;

    return false;
}

}  // namespace Config
