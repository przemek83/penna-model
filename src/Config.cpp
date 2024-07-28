#include "Config.h"

#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>
#include <argparse/argparse.hpp>

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

const std::map<Field, std::string> fieldToString{
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
    os << "check failed: " << fieldToString.at(field) << " " << condition
       << ", got " << currentValue << std::endl;
    return os.str();
}

Config::Mutations loadMutations(const YAML::Node& node)
{
    Config::Mutations mutations;

    if (const YAML::Node value{node[fieldToString.at(Field::MUTATIONS_ADDED)]})
        mutations.added_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString.at(Field::MUTATIONS_LETHAL)]})
        mutations.lethal_ = value.as<int>();

    if (const YAML::Node value{
            node[fieldToString.at(Field::MUTATIONS_INITIAL)]})
        mutations.initial_ = value.as<int>();

    return mutations;
}

Config::Offspring loadOffspring(const YAML::Node& node)
{
    Config::Offspring offspring;

    if (const YAML::Node value{node[fieldToString.at(Field::OFFSPRING_CHANCE)]})
        offspring.chance_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString.at(Field::OFFSPRING_COUNT)]})
        offspring.count_ = value.as<int>();

    return offspring;
}

Config::Population loadPopulation(const YAML::Node& node)
{
    Config::Population population;

    if (const YAML::Node value{
            node[fieldToString.at(Field::POPULATION_INITIAL)]})
        population.initial_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString.at(Field::POPULATION_MAX)]})
        population.max_ = value.as<int>();

    return population;
}

Config::Catching loadCatching(const YAML::Node& node)
{
    Config::Catching catching;

    if (const YAML::Node value{node[fieldToString.at(Field::CATCHING_PERCENT)]})
        catching.percent_ = value.as<int>();

    if (const YAML::Node value{
            node[fieldToString.at(Field::CATCHING_FROM_YEAR)]})
        catching.fromYear_ = value.as<int>();

    if (const YAML::Node value{
            node[fieldToString.at(Field::CATCHING_FROM_AGE)]})
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
        errorMsg += createErrorMsg(
            Field::POPULATION_INITIAL,
            "lover than " + fieldToString.at(Field::POPULATION_MAX),
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

[[noreturn]] void exitWithMsg(const std::exception& e,
                              const argparse::ArgumentParser& parser)
{
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    abort();
}

void fillParser(argparse::ArgumentParser& parser)
{
    parser.add_argument("config")
        .default_value(std::string("config.yaml"))
        .help("name of config file to use");
    parser.add_argument("-p", "--prefix")
        .default_value(std::string("averages"))
        .help("prefix used for output file names");
    parser.add_description(
        "Implementation of Penna model of population aging.");
    parser.add_epilog(
        "Run outputs results into .txt files. You may use scripts "
        "placed in the 'plots' directory to generate plots.");
}

}  // namespace

namespace Config
{
Config::Params loadConfig(std::istream& stream)
{
    Config::Params params;
    YAML::Node yaml{YAML::Load(stream)};

    if (const YAML::Node & node{yaml[fieldToString.at(Field::POPULATION)]};
        node)
        params.population_ = loadPopulation(node);

    if (const YAML::Node value{yaml[fieldToString.at(Field::YEARS)]})
        params.years_ = value.as<int>();

    if (const YAML::Node & node{yaml[fieldToString.at(Field::MUTATIONS)]}; node)
        params.mutations_ = loadMutations(node);

    if (const YAML::Node value{yaml[fieldToString.at(Field::REPRODUCTION_AGE)]})
        params.reproductionAge_ = value.as<int>();

    if (const YAML::Node & node{yaml[fieldToString.at(Field::OFFSPRING)]}; node)
        params.offspring_ = loadOffspring(node);

    if (const YAML::Node value{yaml[fieldToString.at(Field::SIMULATIONS)]})
        params.simulationsCount_ = value.as<int>();

    if (const YAML::Node & node{yaml[fieldToString.at(Field::CATCHING)]}; node)
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

std::pair<std::string, std::string> getAppArguments(int argc, char* argv[])
{
    argparse::ArgumentParser parser("penna-model", "1.0",
                                    argparse::default_arguments::help);
    fillParser(parser);

    std::string configFileName;
    std::string prefix;
    try
    {
        parser.parse_args(argc, argv);
        configFileName = parser.get<std::string>("config");
        prefix = parser.get<std::string>("prefix");
    }
    catch (const std::exception& e)
    {
        exitWithMsg(e, parser);
    }

    return {configFileName, prefix};
}

std::pair<bool, Config::Params> getParams(const std::string& configFileName)
{
    std::ifstream configFileStream(configFileName);
    if (configFileStream.fail())
    {
        std::cerr << "Cannot read config file " + configFileName << std::endl;
        return {false, {}};
    }

    const Config::Params params{Config::loadConfig(configFileStream)};

    if (!Config::isValid(params))
        return {false, {}};

    return {true, params};
}

}  // namespace Config
