#include "Simulation.h"

#include <vector>

#include "Generator.h"
#include "Output.h"

Simulation::Simulation(const Config& config, int number, float step)
    : config_(config), number_(number), step_{step}
{
}

SingleSimulationData Simulation::run(
    Generator& generator, Output& output,
    const std::function<void(std::size_t)> progressCallback)
{
    createInitialPopulation(generator);
    output.saveInitialPopulation(individuals_);

    std::vector<BasicMetrics> basicMetrics;
    basicMetrics.reserve(config_.years_);

    std::size_t year{0};
    while (year < config_.years_)
    {
        const bool singleFamily{isSingleFamily(year, basicMetrics)};
        const int livesAtStart{getLivesOnYearStart(year, basicMetrics)};
        const BasicMetrics yearMetrics{
            progressByOneYear(generator, singleFamily, livesAtStart)};

        basicMetrics.push_back(yearMetrics);

        year++;

        if (progressCallback)
            progressCallback(year);
    }

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
    BasicMetrics yearMetrics{singleFamily ? 1 : 0, livesAtStart, 0, 0};

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
            for (int i{0}; i < config_.offspringCount_; i++)
                individuals_.emplace_front(
                    individual.offspring(generator, config_.mutationsDelta_));
            yearMetrics.births_ += config_.offspringCount_;
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
    std::vector<BasicMetrics> basicMetrics,
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

bool Simulation::isSingleFamily(std::size_t year,
                                const std::vector<BasicMetrics>& basicMetrics)
{
    return year == 0 ? false : basicMetrics[year - 1].families_ == 1;
}

int Simulation::getLivesOnYearStart(
    std::size_t year, const std::vector<BasicMetrics>& basicMetrics) const
{
    return year == 0 ? config_.livesOnStart_
                     : basicMetrics[year - 1].getLivingAtEnd();
}
