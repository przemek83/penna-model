#include <algorithm>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "catch2/matchers/catch_matchers_string.hpp"

#include <src/Individual.h>

#include "FakeGenerator.h"
#include "MockedGenerator.h"

namespace
{
std::size_t getBitPositionInGenome(std::size_t genomeSize, std::size_t positon)
{
    return genomeSize - 1 - positon;
}

void setGenome(Individual& individual, std::string genome)
{
    std::list<int> mutations;
    const std::size_t genomeSize{genome.size()};
    for (std::size_t i{0}; i < genomeSize; ++i)
        if (genome[i] == '1')
            mutations.push_back(
                static_cast<int>(getBitPositionInGenome(genome.size(), i)));

    FakeGenerator fakeGenerator{mutations};
    individual.assignRandomBits(fakeGenerator,
                                static_cast<int>(mutations.size()));
}
}  // namespace

TEST_CASE("Individual", "[penna]")
{
    const int bits{config::Params::bits_};

    SECTION("bit strings creation")
    {
        MockedGenerator generator(bits);
        Individual individual(0);
        individual.assignRandomBits(generator, 4);
        REQUIRE_THAT(
            individual.asBitString(),
            Catch::Matchers::Equals("000010000000000000000000001000000100000000"
                                    "0000000000000000000001"));
    }

    Individual individual(0);

    SECTION("no mutation applied")
    {
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 0)};

        std::string childGenome{child.asBitString()};
        REQUIRE(std::count(childGenome.begin(), childGenome.end(), '1') == 0);
    }

    SECTION("single mutation applied")
    {
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 1)};

        std::string childGenome{child.asBitString()};
        REQUIRE(std::count(childGenome.begin(), childGenome.end(), '1') == 1);
    }

    SECTION("single mutation applied on existing")
    {
        const std::string genomeBeforeMutation{
            "0000000000000000000000000000000000000000000000000000000000001000"};
        setGenome(individual, genomeBeforeMutation);
        FakeGenerator generator({3});
        const Individual child{individual.offspring(generator, 1)};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(genomeBeforeMutation));
    }

    SECTION("single mutation exact match")
    {
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 1)};

        const std::string expectedGenome{
            "0000100000000000000000000000000000000000000000000000000000000000"};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }

    SECTION("multiple mutations exact match")
    {
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 3)};

        const std::string expectedGenome{
            "0000100000000000000000000010000001000000000000000000000000000000"};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }

    SECTION("get genome bits")
    {
        const std::string genome{
            "0000000000000000010000000000000000000000100000000000001000001000"};
        setGenome(individual, genome);
        const std::size_t genomeSize{genome.size()};
        for (size_t i{0}; i < genomeSize; ++i)
            REQUIRE((genome[getBitPositionInGenome(genome.size(), i)] == '1') ==
                    individual.getGenomeBit(i));
    }

    const std::string genomeString{
        "000000000000000001000000000000000000000000"
        "0000100100000000001000"};
    setGenome(individual, genomeString);

    SECTION("ageing passing 0")
    {
        individual.ageByOneYear();
        REQUIRE(individual.getSurvivedMutations() == 0);
    }

    SECTION("ageing passing 1")
    {
        for (int i{0}; i <= 3; ++i)
            individual.ageByOneYear();
        REQUIRE(individual.getSurvivedMutations() == 1);
    }

    SECTION("ageing passing 0 after 1")
    {
        for (int i{0}; i <= 4; ++i)
            individual.ageByOneYear();
        individual.ageByOneYear();
        REQUIRE(individual.getSurvivedMutations() == 1);
    }

    SECTION("ageing passing multiple 1s")
    {
        for (int i{0}; i <= 14; ++i)
            individual.ageByOneYear();
        REQUIRE(individual.getSurvivedMutations() == 2);
    }

    SECTION("ageing passing all bits")
    {
        for (unsigned int i{1}; i < bits; ++i)
            individual.ageByOneYear();
        REQUIRE(individual.getSurvivedMutations() == 4);
    }

    SECTION("bits as string")
    {
        REQUIRE_THAT(individual.asBitString(),
                     Catch::Matchers::Equals(genomeString));
    }

    SECTION("offspring check bits")
    {
        MockedGenerator generator(bits);
        individual.assignRandomBits(generator, 4);
        const std::string parentBits{individual.asBitString()};

        const Individual child{individual.offspring(generator, 0)};

        REQUIRE_THAT(child.asBitString(), Catch::Matchers::Equals(parentBits));
    }

    SECTION("offspring check age")
    {
        for (unsigned int i{0}; i < 10; ++i)
            individual.ageByOneYear();
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 0)};

        REQUIRE(child.getAge() == 0);
    }

    SECTION("offspring ancestor")
    {
        MockedGenerator generator(bits);
        individual = Individual(77);
        const Individual child{individual.offspring(generator, 0)};

        REQUIRE(child.getAncestor() == 77);
    }

    SECTION("offspring survived ones")
    {
        MockedGenerator generator(bits);
        for (int i{0}; i <= 14; ++i)
            individual.ageByOneYear();
        const Individual child{individual.offspring(generator, 0)};

        REQUIRE(child.getSurvivedMutations() == 0);
    }

    individual = Individual(0);
    SECTION("should live - newborn") { REQUIRE_FALSE(individual.shouldDie(1)); }

    SECTION("should live - adult")
    {
        for (int i{0}; i <= 30; ++i)
            individual.ageByOneYear();
        REQUIRE_FALSE(individual.shouldDie(1));
    }

    setGenome(individual, genomeString);
    SECTION("should live - multiple mutations")
    {
        for (int i{0}; i < (config::Params::bits_ - 1); ++i)
            individual.ageByOneYear();
        REQUIRE_FALSE(individual.shouldDie(5));
    }

    SECTION("should die by age")
    {
        for (int i{0}; i < config::Params::bits_; ++i)
            individual.ageByOneYear();
        REQUIRE(individual.shouldDie(5));
    }

    SECTION("should die by lethal mutations")
    {
        for (int i{0}; i < (config::Params::bits_ - 1); ++i)
            individual.ageByOneYear();
        REQUIRE(individual.shouldDie(4));
    }
}
