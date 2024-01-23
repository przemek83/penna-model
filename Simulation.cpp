#include "Simulation.h"

#include <iostream>
#include <vector>

#include "Generator.h"
#include "Output.h"

Simulation::Simulation(const Config& config, int number, float step)
    : config_(config), number_(number), step_{step}
{
}

void Simulation::run(Output& output, Generator& generator,
                     SimulationData::AvgData& simulationDataAvg)
{
    int year{0};
    bool singleFamilyLeft{false};
    int populationCount{config_.livesOnStart_};

    createInitialPopulation(generator);
    output.saveInitialPopulation(individuals_);

    printf("%d/%d Progress:       [", number_, config_.simulationsCount_);

    std::array<int, Config::bits_> gompertzDeathsDistribution{};
    std::array<int, Config::bits_> gompertzAgeDistribution{};

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

        output.zapisz_kolejne(singleFamilyLeft, year, simulationDataAvg,
                              populationCount, ilosc_narodzin, familiesCount,
                              zgon);

        year++;
        if ((year % (config_.years_ / 50)) == 0)
            std::cout << "*";
    }

    const std::array<int, Config::bits_> ageDistribution{
        getAgeDistribution(individuals_)};
    const std::array<int, Config::bits_> bitsDistribution{
        getBitsDistribution(individuals_)};

    output.saveAgeDistribution(ageDistribution);
    output.saveBitsDistribution(bitsDistribution, populationCount);
    output.saveDeathsDistribution(gompertzDeathsDistribution,
                                  gompertzAgeDistribution);

    updateAvgDistributions(simulationDataAvg, ageDistribution, bitsDistribution,
                           gompertzDeathsDistribution, gompertzAgeDistribution,
                           populationCount);

    std::cout << "]";
    output.zapisz_koncowa_populacje(individuals_,
                                    config_.simulationsCount_ + 1 - number_);
}

void Simulation::createInitialPopulation(Generator& generator)
{
    for (size_t i{0}; i < config_.livesOnStart_; i++)
    {
        Individual individual(i);
        individual.assignRandomBits(generator, config_.startingMutations_);
        individuals_.emplace_back(std::move(individual));
    }
}

std::array<int, Config::bits_> Simulation::getAgeDistribution(
    const std::list<Individual>& individuals)
{
    std::array<int, Config::bits_> ageDistribution{};
    for (const auto& individual : individuals)
        ageDistribution[individual.getAge()]++;
    return ageDistribution;
}

std::array<int, Config::bits_> Simulation::getBitsDistribution(
    const std::list<Individual>& individuals)
{
    std::array<int, Config::bits_> bitsDistribution{};
    for (const auto& individual : individuals)
    {
        for (size_t i = 0; i < Config::bits_; i++)
            if (individual.genome_[i])
                bitsDistribution[i]++;
    }
    return bitsDistribution;
}

void Simulation::updateAvgDistributions(
    SimulationData::AvgData& simulationDataAvg,
    const std::array<int, Config::bits_>& ageDistribution,
    const std::array<int, Config::bits_>& bitsDistribution,
    const std::array<int, Config::bits_>& gompertzDeathsDistribution,
    const std::array<int, Config::bits_>& gompertzAgeDistribution,
    int populationCount)
{
    for (int v = 0; v < Config::bits_; v++)
    {
        simulationDataAvg.bity[v] +=
            (float)bitsDistribution[v] / (float)populationCount;
        simulationDataAvg.wiek[v] += ageDistribution[v];
        if (gompertzAgeDistribution[v] > 0)
        {
            simulationDataAvg.gompertz[v] +=
                (float)gompertzDeathsDistribution[v] /
                (float)gompertzAgeDistribution[v];
        }
        else
        {
            simulationDataAvg.gompertz[v] += 1;
        }
    }
}
