#include <bitset>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers_string.hpp"

#include "Individual.h"
#include "MockedGenerator.h"

TEST_CASE("Individual", "[penna]")
{
    const std::string genome{
        "000000000000000001000000000000000000000000"
        "0000100100000000001000"};

    SECTION("bit strings creation")
    {
        MockedGenerator generator;
        Individual individual;
        individual.assignRandomBits(generator, 4);
        REQUIRE(
            individual.genome_.to_ullong() ==
            0b0000000000000000010000000000000000000000100000000000001000001000);
    }

    Individual individual;
    individual.genome_ = std::bitset<Config::bits_>(genome);

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
        for (unsigned int i{1}; i < Config::bits_; ++i)
            individual.ageByOneYear();
        REQUIRE(individual.getSurvivedMutations() == 4);
    }

    SECTION("bits as string")
    {
        REQUIRE_THAT(individual.asBitString(), Catch::Matchers::Equals(genome));
    }

    SECTION("offspring check bits")
    {
        MockedGenerator generator;
        individual.assignRandomBits(generator, 4);
        const std::string parentBits{individual.asBitString()};

        const Individual child{individual.offspring()};

        REQUIRE_THAT(child.asBitString(), Catch::Matchers::Equals(parentBits));
    }

    SECTION("offspring check age")
    {
        for (unsigned int i{0}; i < 10; ++i)
            individual.ageByOneYear();
        const Individual child{individual.offspring()};

        REQUIRE(child.getAge() == 0);
    }

    SECTION("offspring ancestor")
    {
        individual.ancestor_ = 77;
        const Individual child{individual.offspring()};

        REQUIRE(child.ancestor_ == 77);
    }

    SECTION("offspring survived ones")
    {
        for (int i{0}; i <= 14; ++i)
            individual.ageByOneYear();
        const Individual child{individual.offspring()};

        REQUIRE(child.getSurvivedMutations() == 0);
    }

    SECTION("single mutation applied")
    {
        individual.genome_ = std::bitset<Config::bits_>(0);
        MockedGenerator generator;
        individual.applyMutation(generator);

        REQUIRE_THAT(individual.asBitString(),
                     !Catch::Matchers::Equals(genome));
    }

    SECTION("single mutation applied on existing")
    {
        individual.genome_ = std::bitset<Config::bits_>(8);
        MockedGenerator generator;
        const std::string genomeBeforeMutation{individual.asBitString()};
        individual.applyMutation(generator);

        REQUIRE_THAT(individual.asBitString(),
                     Catch::Matchers::Equals(genomeBeforeMutation));
    }

    SECTION("single mutation exact match")
    {
        individual.genome_ = std::bitset<Config::bits_>(0);
        MockedGenerator generator;
        individual.applyMutation(generator);

        const std::string expectedGenome{
            "000000000000000000000000000000000000000000"
            "0000000000000000001000"};

        REQUIRE_THAT(individual.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }

    SECTION("multiple mutations exact match")
    {
        individual.genome_ = std::bitset<Config::bits_>(0);
        MockedGenerator generator;
        individual.applyMutation(generator);
        individual.applyMutation(generator);
        individual.applyMutation(generator);

        const std::string expectedGenome{
            "0000000000000000010000000000000000000000100000000000000000001000"};

        REQUIRE_THAT(individual.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }
}
