#include "Simulation.h"

#include <cmath>
#include <vector>

#include "Generator.h"
#include "Output.h"

Simulation::Simulation(const Config::Params& params, float step)
    : params_(params), step_{step}
{
}

SingleSimulationData Simulation::run()
{
    std::vector<BasicMetrics> basicMetrics;
    basicMetrics.reserve(static_cast<std::size_t>(params_.years_));

    int year{0};
    while (year < params_.years_)
    {
        const bool singleFamily{isSingleFamily(year, basicMetrics)};
        const int livesAtStart{getLivesOnYearStart(year, basicMetrics)};
        const BasicMetrics yearMetrics{
            progressByOneYear(singleFamily, livesAtStart)};

        basicMetrics.push_back(yearMetrics);

        if (progressCallback_)
            progressCallback_(year);

        year++;
    }

    SingleSimulationData data{prepareData(std::move(basicMetrics))};

    return data;
}

void Simulation::setProgressCallback(std::function<void(int)> callback)
{
    progressCallback_ = std::move(callback);
}

SingleSimulationData::BasicMetrics Simulation::progressByOneYear(
    bool singleFamily, int livesAtStart)
{
    BasicMetrics yearMetrics{singleFamily ? 1 : 0, livesAtStart, 0, 0};

    std::vector<int> families(static_cast<std::size_t>(params_.livesOnStart_),
                              0);

    const int chanceForDeathInPercent{
        getCurrentDeathChanceInPercent(livesAtStart)};

    auto it{individuals_.begin()};
    while (it != individuals_.end())
    {
        Individual& individual{*it};

        if (int& family{
                families[static_cast<std::size_t>(individual.getAncestor())]};
            !singleFamily && family != 1)
        {
            yearMetrics.families_++;
            family = 1;
        }

        if (shouldDie(individual, chanceForDeathInPercent))
        {
            yearMetrics.deaths_++;
            it = individuals_.erase(it);
            continue;
        }

        if (shouldHaveOffspring(individual))
        {
            for (int i{0}; i < params_.offspringCount_; i++)
                individuals_.emplace_front(individual.offspring(
                    *generator_, params_.mutations_.added_));
            yearMetrics.births_ += params_.offspringCount_;
        }

        individual.ageByOneYear();
        it++;
    }

    return yearMetrics;
}

void Simulation::createInitialPopulation()
{
    for (int i{0}; i < params_.livesOnStart_; i++)
    {
        Individual individual(i);
        individual.assignRandomBits(*generator_, params_.mutations_.initial_);
        individuals_.push_back(individual);
    }
}

void Simulation::saveInitialPopulation(Output& output)
{
    output.saveInitialPopulation(individuals_);
}

void Simulation::saveFinalPopulation(Output& output)
{
    output.saveFinalPopulation(individuals_);
}

void Simulation::setGenerator(std::shared_ptr<Generator> generator)
{
    generator_ = std::move(generator);
}

std::vector<int> Simulation::getAgeDistribution(
    const std::list<Individual>& individuals)
{
    std::vector<int> ageDistribution(Config::Params::bits_, 0);

    for (const auto& individual : individuals)
        ageDistribution[static_cast<std::size_t>(individual.getAge())]++;

    return ageDistribution;
}

std::vector<int> Simulation::getBitsDistribution(
    const std::list<Individual>& individuals)
{
    std::vector<int> bitsDistribution(Config::Params::bits_, 0);

    for (const auto& individual : individuals)
    {
        for (std::size_t i{0}; i < Config::Params::bits_; i++)
            bitsDistribution[i] += static_cast<int>(individual.getGenomeBit(i));
    }

    return bitsDistribution;
}

int Simulation::getCurrentDeathChanceInPercent(int populationCount) const
{
    return static_cast<int>(
        std::round(static_cast<float>(populationCount) /
                   static_cast<float>(params_.maxPopulation_) * 100));
}

bool Simulation::shouldDie(const Individual& individual,
                           int chanceForDeathInPercent) const
{
    return (individual.getSurvivedMutations() >=
            params_.mutations_.lethal_) ||                    // mutations
           (individual.getAge() >= Config::Params::bits_) ||  // ageing
           (generator_->getPercentChance() <=
            chanceForDeathInPercent)                          // Verhulst
#ifdef SYMULACJA_DORSZY
           || ((rok > ODLOWY_OD) && (individual.wiek >= MINIMALNY_WIEK) &&
               ((float)generator.getInt(0, 10000) / 100 <=
                START_ODLOWOW + number_ * step_))
#endif
        ;
}

bool Simulation::shouldHaveOffspring(const Individual& individual) const
{
    return (individual.getAge() > params_.reproductionAge_) &&
           generator_->getPercentChance() <= params_.chanceForOffspring_;
}

SingleSimulationData Simulation::prepareData(
    std::vector<BasicMetrics> basicMetrics) const
{
    const int populationCount{basicMetrics.back().getLivingAtEnd()};
    SingleSimulationData data{static_cast<std::size_t>(params_.years_)};
    data.setBasicMetrics(std::move(basicMetrics));

    const std::vector<int> ageDistribution{getAgeDistribution(individuals_)};
    const std::vector<int> bitsDistribution{getBitsDistribution(individuals_)};

    data.setAgeDistribution(ageDistribution);
    data.setBitDistribution(bitsDistribution, populationCount);

    const auto [deathsDistribution,
                deathsAgeDistribution]{getDeathsDistributionData()};
    data.setDeathDistribution(deathsDistribution, deathsAgeDistribution);
    return data;
}

std::pair<std::vector<int>, std::vector<int> >
Simulation::getDeathsDistributionData() const
{
    std::vector<int> gompertzDeathsDistribution(Config::Params::bits_, 0);
    std::vector<int> gompertzAgeDistribution(Config::Params::bits_, 0);

    const int chanceForDeathInPercent{
        getCurrentDeathChanceInPercent(static_cast<int>(individuals_.size()))};

    auto it{individuals_.begin()};
    while (it != individuals_.end())
    {
        const Individual& individual{*it};
        const std::size_t age{static_cast<std::size_t>(individual.getAge())};

        gompertzAgeDistribution[age]++;
        if (shouldDie(individual, chanceForDeathInPercent))
            gompertzDeathsDistribution[age]++;

        it++;
    }

    return {gompertzDeathsDistribution, gompertzAgeDistribution};
}

bool Simulation::isSingleFamily(int year,
                                const std::vector<BasicMetrics>& basicMetrics)
{
    return year == 0
               ? false
               : basicMetrics[static_cast<std::size_t>(year - 1)].families_ ==
                     1;
}

int Simulation::getLivesOnYearStart(
    int year, const std::vector<BasicMetrics>& basicMetrics) const
{
    return year == 0 ? params_.livesOnStart_
                     : basicMetrics[static_cast<std::size_t>(year - 1)]
                           .getLivingAtEnd();
}
