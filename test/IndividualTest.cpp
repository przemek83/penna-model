#include <bitset>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "catch2/matchers/catch_matchers_string.hpp"

#include <src/Individual.h>
#include "MockedGenerator.h"

TEST_CASE("Individual", "[penna]")
{
    const Config::Params defaultParams;
    const int bits{defaultParams.bits_};

    const std::string genomeString{
        "000000000000000001000000000000000000000000"
        "0000100100000000001000"};

    SECTION("bit strings creation")
    {
        MockedGenerator generator(bits);
        Individual individual(0);
        individual.assignRandomBits(generator, 4);
        individual.asBitString(), Catch::Matchers::Equals(genomeString);
    }

    Individual individual(0);
    individual.setGenome(std::bitset<bits>(genomeString));

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

    SECTION("single mutation applied")
    {
        individual.setGenome(std::bitset<bits>(0));
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 1)};

        REQUIRE_THAT(child.asBitString(),
                     !Catch::Matchers::Equals(genomeString));
    }

    SECTION("single mutation applied on existing")
    {
        individual.setGenome(std::bitset<bits>(8));
        MockedGenerator generator(bits);
        const std::string genomeBeforeMutation{individual.asBitString()};
        const Individual child{individual.offspring(generator, 1)};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(genomeBeforeMutation));
    }

    SECTION("single mutation exact match")
    {
        individual.setGenome(std::bitset<bits>(0));
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 1)};

        const std::string expectedGenome{
            "000000000000000000000000000000000000000000"
            "0000000000000000001000"};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }

    SECTION("multiple mutations exact match")
    {
        individual.setGenome(std::bitset<bits>(0));
        MockedGenerator generator(bits);
        const Individual child{individual.offspring(generator, 3)};

        const std::string expectedGenome{
            "0000000000000000010000000000000000000000100000000000000000001000"};

        REQUIRE_THAT(child.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }

    SECTION("get genome bits")
    {
        const std::bitset<bits> bitset(
            0b0000000000000000010000000000000000000000100000000000001000001000);
        individual.setGenome(bitset);
        for (size_t i{0}; i < bitset.size(); ++i)
            REQUIRE(bitset[i] == individual.getGenomeBit(i));
    }
}
