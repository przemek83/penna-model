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
        // 16384  = 00000000000000000100000000000000
        // 147464 = 00000000000000100100000000001000
        REQUIRE(individual.genome_[0] == 16384);
        REQUIRE(individual.genome_[1] == 147464);
    }

    Individual individual;
    individual.genome_[0] = 16384;
    individual.genome_[1] = 147464;

    SECTION("ageing passing 0")
    {
        individual.ageByOneYear(17);
        REQUIRE(individual.survivedOnes_ == 0);
    }

    SECTION("ageing passing 1")
    {
        individual.ageByOneYear(18);
        REQUIRE(individual.survivedOnes_ == 1);
    }

    SECTION("ageing passing 0 after 1")
    {
        individual.ageByOneYear(18);
        individual.ageByOneYear(19);
        REQUIRE(individual.survivedOnes_ == 1);
    }

    SECTION("ageing passing multiple 1s")
    {
        individual.ageByOneYear(18);
        individual.ageByOneYear(47);
        REQUIRE(individual.survivedOnes_ == 2);
    }

    SECTION("ageing passing all bits")
    {
        for (unsigned int i{0}; i < Config::bits_; ++i)
            individual.ageByOneYear(i);
        REQUIRE(individual.survivedOnes_ == 4);
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
        individual.age_ = 10;
        const Individual child{individual.offspring()};

        REQUIRE(child.age_ == 0);
    }

    SECTION("offspring ancestor")
    {
        individual.ancestor_ = 77;
        const Individual child{individual.offspring()};

        REQUIRE(child.ancestor_ == 77);
    }

    SECTION("offspring survived ones")
    {
        individual.survivedOnes_ = 3;
        const Individual child{individual.offspring()};

        REQUIRE(child.survivedOnes_ == 0);
    }

    SECTION("single mutation applied")
    {
        individual.genome_[0] = 0;
        individual.genome_[1] = 0;
        MockedGenerator generator;
        individual.applyMutation(generator);

        REQUIRE_THAT(individual.asBitString(),
                     !Catch::Matchers::Equals(genome));
    }

    SECTION("single mutation applied on existing")
    {
        individual.genome_[0] = 0;
        individual.genome_[1] = 8;
        MockedGenerator generator;
        const std::string genomeBeforeMutation{individual.asBitString()};
        individual.applyMutation(generator);

        REQUIRE_THAT(individual.asBitString(),
                     Catch::Matchers::Equals(genomeBeforeMutation));
    }

    SECTION("single mutation exact match")
    {
        individual.genome_[0] = 0;
        individual.genome_[1] = 0;
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
        individual.genome_[0] = 0;
        individual.genome_[1] = 0;
        MockedGenerator generator;
        individual.applyMutation(generator);
        individual.applyMutation(generator);
        individual.applyMutation(generator);

        const std::string expectedGenome{
            "0000000000000000010000000000000000000000000000000100000000001000"};

        REQUIRE_THAT(individual.asBitString(),
                     Catch::Matchers::Equals(expectedGenome));
    }
}
