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
    int year{0};
    bool singleFamilyLeft{false};
    int populationCount{config_.livesOnStart_};

    createInitialPopulation(generator);
    output.saveInitialPopulation(individuals_);

    printf("%d/%d Progress:       [", number_, config_.simulationsCount_);

    std::vector<int> gompertzDeathsDistribution;
    gompertzDeathsDistribution.resize(config_.bits_, 0);
    std::vector<int> gompertzAgeDistribution;
    gompertzAgeDistribution.resize(config_.bits_, 0);

    std::vector<SingleSimulationData::BasicData> basicData;
    basicData.resize(config_.years_);

    while (year < config_.years_)
    {
        int ilosc_narodzin{0};
        int familiesCount{0};
        int zgon{0};
        std::vector<int> families;
        families.resize(config_.livesOnStart_, 0);

#ifdef SYMULACJA_DORSZY
        if (rok + 1 == MAX_POP_LAT)
        {
            if (ostatni_el - puste.size() == 0)
                return;
        }
#endif

        const int chanceForDeath{static_cast<int>(
            (float)populationCount / config_.maxPopulation_ * 100.0)};

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

            if ((individual.getSurvivedMutations() >=
                 config_.maxMutations_) ||                    // mutations
                (individual.getAge() >= Config::bits_) ||     // ageing
                (generator.getInt(0, 100) <= chanceForDeath)  // verhulst
#ifdef SYMULACJA_DORSZY
                || ((rok > ODLOWY_OD) && (individual.wiek >= MINIMALNY_WIEK) &&
                    ((float)generator.getInt(0, 10000) / 100 <=
                     START_ODLOWOW + number_ * step_))
#endif
            )
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

        basicData[year].families_ = familiesCount;
        basicData[year].livingAtStart_ = populationCount;
        basicData[year].births_ = ilosc_narodzin;
        basicData[year].livingAtEnd_ = populationCount - zgon;
        basicData[year].deaths_ = zgon;

        year++;
        if ((year % (config_.years_ / 50)) == 0)
            std::cout << "*";
    }

    SingleSimulationData data{static_cast<std::size_t>(config_.years_)};
    data.setBasicData(std::move(basicData));
    output.saveBasicSimulationMetrics(data);

    const std::vector<int> ageDistribution{getAgeDistribution(individuals_)};
    const std::vector<int> bitsDistribution{getBitsDistribution(individuals_)};

    data.setAgeDistribution(ageDistribution);
    data.setBitDistribution(bitsDistribution, populationCount);
    data.setDeathDistribution(gompertzDeathsDistribution,
                              gompertzAgeDistribution);

    output.saveDeathsDistribution(data.getDeathsDistribution());
    output.saveBitsDistribution(data.getBitsDistribution());
    output.saveAgeDistribution(data.getAgeDistribution());

    std::cout << "]";

    output.saveFinalPopulation(individuals_);

    return data;
}

void Simulation::createInitialPopulation(Generator& generator)
{
    for (std::size_t i{0}; i < static_cast<std::size_t>(config_.livesOnStart_);
         i++)
    {
        Individual individual(i);
        individual.assignRandomBits(generator, config_.startingMutations_);
        individuals_.emplace_back(std::move(individual));
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
