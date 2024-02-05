#include "Simulation.h"

#include <iostream>
#include <vector>

#include "Generator.h"
#include "Output.h"

Simulation::Simulation(const Config& config, int number, float step)
    : config_(config), number_(number), step_{step}
{
}

SingleSimulationData Simulation::run(Generator& generator, Output& output)
{
    createInitialPopulation(generator);
    output.saveInitialPopulation(individuals_);

    std::cout << number_ << "/" << config_.simulationsCount_
              << " Progress:       [";

    std::vector<SingleSimulationData::BasicMetrics> basicMetrics;
    basicMetrics.reserve(config_.years_);

    std::size_t year{0};
    while (year < config_.years_)
    {
        const bool singleFamily{
            year == 0 ? false : basicMetrics[year - 1].families_ == 1};
        const int livesAtStart{year == 0
                                   ? config_.livesOnStart_
                                   : basicMetrics[year - 1].getLivingAtEnd()};

        const SingleSimulationData::BasicMetrics yearMetrics{
            progressByOneYear(generator, singleFamily, livesAtStart)};

        basicMetrics.push_back(yearMetrics);

        year++;
        if ((year % (config_.years_ / 50)) == 0)
            std::cout << "*";
    }

    std::cout << "]";

    const auto [deathsDistribution,
                ageDistribution]{getDeathsDistributionData(generator)};

    SingleSimulationData data{prepareData(std::move(basicMetrics),
                                          deathsDistribution, ageDistribution)};

    saveSimulationData(data, output);

    return data;
}

SingleSimulationData::BasicMetrics Simulation::progressByOneYear(
    Generator& generator, bool singleFamily, int livesAtStart)
{
    SingleSimulationData::BasicMetrics yearMetrics{singleFamily ? 1 : 0,
                                                   livesAtStart, 0, 0};

    std::vector<int> families(config_.livesOnStart_, 0);

    const int chanceForDeathInPercent{
        getCurrentDeathChanceInPercent(livesAtStart)};

    auto it{individuals_.begin()};
    while (it != individuals_.end())
    {
        Individual& individual{*it};

        if (int& family{families[individual.getAncestor()]};
            !singleFamily && family != 1)
        {
            yearMetrics.families_++;
            family = 1;
        }

        if (shouldDie(individual, generator, chanceForDeathInPercent))
        {
            yearMetrics.deaths_++;
            it = individuals_.erase(it);
            continue;
        }

        if (shouldHaveOffspring(individual, generator))
        {
            for (int l{0}; l < config_.offspringCount_; l++)
            {
                Individual osobnik{individual.offspring()};
                yearMetrics.births_++;

                for (int m{0}; m < config_.mutationsDelta_; m++)
                    osobnik.applyMutation(generator);

                individuals_.push_front(osobnik);
            }
        }

        individual.ageByOneYear();
        it++;
    }

    return yearMetrics;
}

void Simulation::createInitialPopulation(Generator& generator)
{
    for (int i{0}; i < config_.livesOnStart_; i++)
    {
        Individual individual(i);
        individual.assignRandomBits(generator, config_.startingMutations_);
        individuals_.push_back(individual);
    }
}

std::vector<int> Simulation::getAgeDistribution(
    const std::list<Individual>& individuals)
{
    std::vector<int> ageDistribution(Config::bits_, 0);

    for (const auto& individual : individuals)
        ageDistribution[static_cast<std::size_t>(individual.getAge())]++;

    return ageDistribution;
}

std::vector<int> Simulation::getBitsDistribution(
    const std::list<Individual>& individuals)
{
    std::vector<int> bitsDistribution(Config::bits_, 0);

    for (const auto& individual : individuals)
    {
        for (size_t i{0}; i < Config::bits_; i++)
            bitsDistribution[i] += static_cast<int>(individual.getGenomeBit(i));
    }

    return bitsDistribution;
}

int Simulation::getCurrentDeathChanceInPercent(int populationCount) const
{
    return static_cast<int>(
        std::round(static_cast<float>(populationCount) /
                   static_cast<float>(config_.maxPopulation_) * 100));
}

bool Simulation::shouldDie(const Individual& individual, Generator& generator,
                           int chanceForDeathInPercent) const
{
    return (individual.getSurvivedMutations() >=
            config_.maxMutations_) ||                             // mutations
           (individual.getAge() >= Config::bits_) ||              // ageing
           (generator.getInt(0, 100) <= chanceForDeathInPercent)  // Verhulst
#ifdef SYMULACJA_DORSZY
           || ((rok > ODLOWY_OD) && (individual.wiek >= MINIMALNY_WIEK) &&
               ((float)generator.getInt(0, 10000) / 100 <=
                START_ODLOWOW + number_ * step_))
#endif
        ;
}

bool Simulation::shouldHaveOffspring(const Individual& individual,
                                     Generator& generator) const
{
    return (individual.getAge() > config_.reproductionAge_) &&
           generator.getInt(1, 100) <= config_.chanceForOffspring_;
}

SingleSimulationData Simulation::prepareData(
    std::vector<SingleSimulationData::BasicMetrics> basicMetrics,
    const std::vector<int>& gompertzDeathsDistribution,
    const std::vector<int>& gompertzAgeDistribution) const
{
    const int populationCount{basicMetrics.back().getLivingAtEnd()};
    SingleSimulationData data{static_cast<std::size_t>(config_.years_)};
    data.setBasicMetrics(std::move(basicMetrics));

    const std::vector<int> ageDistribution{getAgeDistribution(individuals_)};
    const std::vector<int> bitsDistribution{getBitsDistribution(individuals_)};

    data.setAgeDistribution(ageDistribution);
    data.setBitDistribution(bitsDistribution, populationCount);
    data.setDeathDistribution(gompertzDeathsDistribution,
                              gompertzAgeDistribution);
    return data;
}

void Simulation::saveSimulationData(const SingleSimulationData& data,
                                    Output& output) const
{
    output.saveBasicSimulationMetrics(data);
    output.saveDeathsDistribution(data.getDeathsDistribution());
    output.saveBitsDistribution(data.getBitsDistribution());
    output.saveAgeDistribution(data.getAgeDistribution());
    output.saveFinalPopulation(individuals_);
}

std::pair<std::vector<int>, std::vector<int> >
Simulation::getDeathsDistributionData(Generator& generator) const
{
    std::vector<int> gompertzDeathsDistribution(Config::bits_, 0);
    std::vector<int> gompertzAgeDistribution(Config::bits_, 0);

    const int chanceForDeathInPercent{
        getCurrentDeathChanceInPercent(individuals_.size())};

    auto it{individuals_.begin()};
    while (it != individuals_.end())
    {
        const Individual& individual{*it};
        const std::size_t age{static_cast<std::size_t>(individual.getAge())};

        gompertzAgeDistribution[age]++;
        if (shouldDie(individual, generator, chanceForDeathInPercent))
            gompertzDeathsDistribution[age]++;

        it++;
    }

    return {gompertzDeathsDistribution, gompertzAgeDistribution};
}
