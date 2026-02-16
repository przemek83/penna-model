#include "Config.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include <cpputils/Logger.h>
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

constexpr std::string_view fieldToString(Field field)
{
    switch (field)
    {
        case Field::POPULATION:
            return "population";
        case Field::POPULATION_INITIAL:
            return "initial";
        case Field::POPULATION_MAX:
            return "max";
        case Field::YEARS:
            return "years";
        case Field::MUTATIONS:
            return "mutations";
        case Field::MUTATIONS_ADDED:
            return "added";
        case Field::MUTATIONS_LETHAL:
            return "lethal";
        case Field::MUTATIONS_INITIAL:
            return "initial";
        case Field::REPRODUCTION_AGE:
            return "reproductionAge";
        case Field::OFFSPRING:
            return "offspring";
        case Field::OFFSPRING_CHANCE:
            return "chance";
        case Field::OFFSPRING_COUNT:
            return "count";
        case Field::SIMULATIONS:
            return "simulations";
        case Field::CATCHING:
            return "catching";
        case Field::CATCHING_PERCENT:
            return "percent";
        case Field::CATCHING_FROM_YEAR:
            return "fromYear";
        case Field::CATCHING_FROM_AGE:
            return "fromAge";
    }
    return "";
}

std::string createErrorMsg(Field field, const std::string& condition,
                           int currentValue)
{
    std::ostringstream os;
    os << "check failed: " << fieldToString(field) << " " << condition
       << ", got " << currentValue << "\n";
    return os.str();
}

config::Mutations loadMutations(const YAML::Node& node)
{
    config::Mutations mutations;

    if (const YAML::Node value{node[fieldToString(Field::MUTATIONS_ADDED)]})
        mutations.added_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString(Field::MUTATIONS_LETHAL)]})
        mutations.lethal_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString(Field::MUTATIONS_INITIAL)]})
        mutations.initial_ = value.as<int>();

    return mutations;
}

config::Offspring loadOffspring(const YAML::Node& node)
{
    config::Offspring offspring;

    if (const YAML::Node value{node[fieldToString(Field::OFFSPRING_CHANCE)]})
        offspring.chance_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString(Field::OFFSPRING_COUNT)]})
        offspring.count_ = value.as<int>();

    return offspring;
}

config::Population loadPopulation(const YAML::Node& node)
{
    config::Population population;

    if (const YAML::Node value{node[fieldToString(Field::POPULATION_INITIAL)]})
        population.initial_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString(Field::POPULATION_MAX)]})
        population.max_ = value.as<int>();

    return population;
}

config::Catching loadCatching(const YAML::Node& node)
{
    config::Catching catching;

    if (const YAML::Node value{node[fieldToString(Field::CATCHING_PERCENT)]})
        catching.percent_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString(Field::CATCHING_FROM_YEAR)]})
        catching.fromYear_ = value.as<int>();

    if (const YAML::Node value{node[fieldToString(Field::CATCHING_FROM_AGE)]})
        catching.fromAge_ = value.as<int>();

    return catching;
}

void checkMutations(config::Mutations mutations, std::string& errorMsg)
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

    if (mutations.initial_ > config::Params::bits_)
        errorMsg += createErrorMsg(
            Field::MUTATIONS_INITIAL,
            "<= " + std::to_string(config::Params::bits_), mutations.initial_);
}

void checkOffspring(config::Offspring offspring, std::string& errorMsg)
{
    if (offspring.count_ < 0)
        errorMsg +=
            createErrorMsg(Field::OFFSPRING_COUNT, ">= 0", offspring.count_);

    if (offspring.chance_ < 0)
        errorMsg +=
            createErrorMsg(Field::OFFSPRING_CHANCE, ">= 0", offspring.chance_);

    if (offspring.chance_ > config::maxPercentage)
        errorMsg += createErrorMsg(
            Field::OFFSPRING_CHANCE,
            "<= " + std::to_string(config::maxPercentage), offspring.chance_);
}

void checkPopulation(config::Population population, std::string& errorMsg)
{
    if (population.max_ <= 0)
        errorMsg +=
            createErrorMsg(Field::POPULATION_MAX, "> 0", population.max_);

    if (population.initial_ <= 0)
        errorMsg += createErrorMsg(Field::POPULATION_INITIAL, "> 0",
                                   population.initial_);

    if (population.initial_ > population.max_)
    {
        auto condition = std::string("lower than ") +
                         std::string(fieldToString(Field::POPULATION_MAX));
        errorMsg += createErrorMsg(Field::POPULATION_INITIAL, condition,
                                   population.initial_);
    }
}

void checkCatching(config::Catching catching, int years, std::string& errorMsg)
{
    if (catching.percent_ < 0)
        errorMsg +=
            createErrorMsg(Field::CATCHING_PERCENT, ">= 0", catching.percent_);

    if (catching.percent_ > config::maxPercentage)
        errorMsg += createErrorMsg(
            Field::CATCHING_PERCENT,
            "<= " + std::to_string(config::maxPercentage), catching.percent_);

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

    if (catching.fromAge_ > config::Params::bits_)
        errorMsg += createErrorMsg(Field::CATCHING_FROM_AGE,
                                   "< " + std::to_string(config::Params::bits_),
                                   catching.fromAge_);
}

void printErrorMsg(const std::exception& e,
                   const argparse::ArgumentParser& parser)
{
    Log().error(std::string(e.what()) + "\n" + parser.help().str());
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
        "Implementation of the Penna model of population aging.");
    parser.add_epilog(
        "Run the output results into .txt files. You may use scripts placed in "
        "the 'plots' directory to generate plots.");
}

}  // namespace

namespace config
{
config::Params loadConfig(std::unique_ptr<std::istream> configStream)
{
    config::Params params;
    YAML::Node yaml{YAML::Load(*configStream)};

    if (const YAML::Node & node{yaml[fieldToString(Field::POPULATION)]}; node)
        params.population_ = loadPopulation(node);

    if (const YAML::Node value{yaml[fieldToString(Field::YEARS)]})
        params.years_ = value.as<int>();

    if (const YAML::Node & node{yaml[fieldToString(Field::MUTATIONS)]}; node)
        params.mutations_ = loadMutations(node);

    if (const YAML::Node value{yaml[fieldToString(Field::REPRODUCTION_AGE)]})
        params.reproductionAge_ = value.as<int>();

    if (const YAML::Node & node{yaml[fieldToString(Field::OFFSPRING)]}; node)
        params.offspring_ = loadOffspring(node);

    if (const YAML::Node value{yaml[fieldToString(Field::SIMULATIONS)]})
        params.simulationsCount_ = value.as<int>();

    if (const YAML::Node & node{yaml[fieldToString(Field::CATCHING)]}; node)
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

    Log().error("Configuration is invalid:\n" + errorMsg);

    return false;
}

std::tuple<bool, std::string, std::string> getAppArguments(int argc,
                                                           const char* argv[])
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
    catch (const std::runtime_error& e)
    {
        printErrorMsg(e, parser);
        return {false, configFileName, prefix};
    }

    return {true, configFileName, prefix};
}

std::pair<bool, config::Params> getParams(
    const std::string& configFileName,
    std::unique_ptr<std::istream> configStream)
{
    if (configStream->fail())
    {
        Log().error("Cannot read config file " + configFileName + "\n");
        return {false, {}};
    }

    const config::Params params{config::loadConfig(std::move(configStream))};

    if (!config::isValid(params))
        return {false, {}};

    return {true, params};
}

}  // namespace config
