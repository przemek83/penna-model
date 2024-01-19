#include "Individual.h"

#include <bitset>
#include <cstdlib>
#include <cstring>

#include "Generator.h"

void Individual::ageByOneYear(unsigned int miejsce)
{
    unsigned long int pomoc = genome_[miejsce / Config::intSize_];
    pomoc = pomoc >> (Config::intSize_ - (miejsce % Config::intSize_));
    if (pomoc & 1)
        survivedOnes_++;
}

std::string Individual::asBitString() const
{
    unsigned long long bitsAsInt{static_cast<unsigned int>(genome_[0])};
    bitsAsInt = bitsAsInt << static_cast<unsigned int>(Config::intSize_);
    bitsAsInt += static_cast<unsigned int>(genome_[1]);
    const std::bitset<Config::bits_> asBitset(bitsAsInt);
    return asBitset.to_string();
}

Individual Individual::offspring() const
{
    Individual offspring{*this};
    offspring.age_ = 0;
    offspring.survivedOnes_ = 0;
    return offspring;
}

void Individual::assignRandomBits(Generator& generator, int startingMutations)
{
    std::array<int, Config::intCount_> newBorn{0, 0};
    int ktore{0};
    int temp{0};

    for (int j = 0; j < startingMutations; j++)
    {
        do
        {
            const int liczba_losowa{generator.getInt(0, Config::intSize_ - 1)};
            ktore = generator.getInt(0, Config::intCount_ - 1);
            temp = 1;
            temp <<= liczba_losowa;
        } while (newBorn[ktore] == (newBorn[ktore] | temp));

        newBorn[ktore] = (newBorn[ktore] | temp);
    }

    for (size_t i = 0; i < Config::intCount_; i++)
        genome_[i] = newBorn[i];
}
