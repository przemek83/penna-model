#include <bitset>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "catch2/matchers/catch_matchers_string.hpp"

#include <src/Individual.h>
#include "MockedGenerator.h"

TEST_CASE("Individual", "[penna]")
{
    const std::string genomeString{
        "000000000000000001000000000000000000000000"
        "0000100100000000001000"};

    SECTION("bit strings creation")
    {
        MockedGenerator generator;
        Individual individual(0);
        individual.assignRandomBits(generator, 4);
        REQUIRE(
            individual.getGenomeAsNumber() ==
            0b0000000000000000010000000000000000000000100000000000001000001000);
    }

    Individual individual(0);
    individual.setGenome(std::bitset<Config::Params::bits_>(genomeString));

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
        for (unsigned int i{1}; i < Config::Params::bits_; ++i)
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
        MockedGenerator generator;
        individual.assignRandomBits(generator, 4);
        const std::string parentBits{individual.asBitString()};

        const Individual child{individual.offspring(generator, 0)};

        REQUIRE_THAT(child.asBitString(), Catch::Matchers::Equals(parentBits));
    }

    SECTION("offspring check age")
    {
        for (unsigned int i{0}; i < 10; ++i)
            individual.ageByOneYear();
        MockedGenerator generator;
        const Individual child{individual.offspring(generator, 0)};

        REQUIRE(child.getAge() == 0);
    }

    SECTION("offspring ancestor")
    {
        MockedGenerator generator;
        individual = Individual(77);
        const Individual child{individual.offspring(generator, 0)};

        REQUIRE(child.getAncestor() == 77);
    }

    SECTION("offspring survived ones")
    {
        MockedGenerator generator;
        for (int i{0}; i <= 14; ++i)
            individual.ageByOneYear();
        const Individual child{individual.offspring(generator, 0)};

        REQUIRE(child.getSurvivedMutations() == 0);
    }

    SECTION("single mutation applied")
    {
        individual.setGenome(std::bitset<Config::Params::bits_>(0));
        MockedGenerator generator;
        const Individual child{individual.offspring(generator, 1)};

        REQUIRE_THAT(child.asBitString(),
                     !Catch::Matchers::Equals(genomeString));
    }

    SECTION("single mutation applied on existing")
    {
        individual.setGenome(std::bitset<Config::Params::bits_>(8));
        MockedGenerator generator;
        const std::string genomeBeforeMutation{individual.asBitString()};
        const Individual child{individual.offspring(generator, 1)};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(genomeBeforeMutation));
    }

    SECTION("single mutation exact match")
    {
        individual.setGenome(std::bitset<Config::Params::bits_>(0));
        MockedGenerator generator;
        const Individual child{individual.offspring(generator, 1)};

        const std::string expectedGenome{
            "000000000000000000000000000000000000000000"
            "0000000000000000001000"};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }

    SECTION("multiple mutations exact match")
    {
        individual.setGenome(std::bitset<Config::Params::bits_>(0));
        MockedGenerator generator;
        const Individual child{individual.offspring(generator, 3)};

        const std::string expectedGenome{
            "0000000000000000010000000000000000000000100000000000000000001000"};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }

    SECTION("genome as number")
    {
        unsigned long long number{GENERATE(0ULL, 1ULL, 21354634ULL)};
        individual.setGenome(std::bitset<Config::Params::bits_>(number));
        REQUIRE(individual.getGenomeAsNumber() == number);
    }

    SECTION("get genome bits")
    {
        const std::bitset<Config::Params::bits_> bitset(
            0b0000000000000000010000000000000000000000100000000000001000001000);
        individual.setGenome(bitset);
        for (size_t i{0}; i < bitset.size(); ++i)
            REQUIRE(bitset[i] == individual.getGenomeBit(i));
    }
}
