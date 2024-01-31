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
    std::size_t year{0};
    bool singleFamilyLeft{false};
    int populationCount{config_.livesOnStart_};

    createInitialPopulation(generator);
    output.saveInitialPopulation(individuals_);

    std::cout << number_ << "/" << config_.simulationsCount_
              << " Progress:       [";

    std::vector<int> gompertzDeathsDistribution;
    gompertzDeathsDistribution.resize(Config::bits_, 0);
    std::vector<int> gompertzAgeDistribution;
    gompertzAgeDistribution.resize(Config::bits_, 0);

    std::vector<SingleSimulationData::BasicData> basicData;

    while (year < config_.years_)
    {
        int ilosc_narodzin{0};
        int familiesCount{0};
        int zgon{0};
        std::vector<int> families;
        families.resize(config_.livesOnStart_, 0);

        const int chanceForDeathInPercent{
            getCurrentDeathChanceInPercent(populationCount)};

        auto it{individuals_.begin()};
        while (it != individuals_.end())
        {
            Individual& individual{*it};

            if (!singleFamilyLeft)
            {
                families[individual.getAncestor()]++;
                if (families[individual.getAncestor()] == 1)
                    familiesCount++;
            }

            if (year + 1 == config_.years_)
                gompertzAgeDistribution[individual.getAge()]++;

            if (shouldDie(individual, generator, chanceForDeathInPercent))
            {
                zgon++;
                if (year + 1 == config_.years_)
                    gompertzDeathsDistribution[individual.getAge()]++;
                it = individuals_.erase(it);
                continue;
            }

            if ((individual.getAge() > config_.reproductionAge_) &&
                generator.getInt(1, 100) <= config_.chanceForOffspring_)
            {
                for (int l{0}; l < config_.offspringCount_; l++)
                {
                    Individual osobnik{individual.offspring()};
                    ilosc_narodzin++;

                    for (int m{0}; m < config_.mutationsDelta_; m++)
                        osobnik.applyMutation(generator);

                    individuals_.push_front(osobnik);
                }
            }

            individual.ageByOneYear();
            it++;
        }

        populationCount -= zgon;
        populationCount += ilosc_narodzin;
        if (familiesCount == 1)
            singleFamilyLeft = true;

        basicData.push_back({familiesCount, populationCount, ilosc_narodzin,
                             populationCount - zgon, zgon});

        year++;
        if ((year % (config_.years_ / 50)) == 0)
            std::cout << "*";
    }

    std::cout << "]";

    SingleSimulationData data{prepareData(std::move(basicData),
                                          gompertzDeathsDistribution,
                                          gompertzAgeDistribution)};

    saveSimulationData(data, output);

    return data;
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
    std::vector<int> ageDistribution;
    ageDistribution.resize(Config::bits_, 0);

    for (const auto& individual : individuals)
        ageDistribution[static_cast<std::size_t>(individual.getAge())]++;

    return ageDistribution;
}

std::vector<int> Simulation::getBitsDistribution(
    const std::list<Individual>& individuals)
{
    std::vector<int> bitsDistribution;
    bitsDistribution.resize(Config::bits_, 0);

    for (const auto& individual : individuals)
    {
        for (size_t i{0}; i < Config::bits_; i++)
            bitsDistribution[i] += static_cast<int>(individual.getGenomeBit(i));
    }

    return bitsDistribution;
}

int Simulation::getCurrentDeathChanceInPercent(int populationCount) const
{
    return std::round(static_cast<float>(populationCount) /
                      static_cast<float>(config_.maxPopulation_) * 100);
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

SingleSimulationData Simulation::prepareData(
    std::vector<SingleSimulationData::BasicData> basicData,
    const std::vector<int>& gompertzDeathsDistribution,
    const std::vector<int>& gompertzAgeDistribution) const
{
    const int populationCount{basicData.back().livingAtStart_};
    SingleSimulationData data{static_cast<std::size_t>(config_.years_)};
    data.setBasicData(std::move(basicData));

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
