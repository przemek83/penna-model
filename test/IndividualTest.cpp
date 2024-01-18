#include <bitset>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers_string.hpp"

#include "Individual.h"
#include "MockedGenerator.h"

TEST_CASE("Individual", "[penna]")
{
    SECTION("bit strings creation")
    {
        MockedGenerator generator;
        Individual individual;
        individual.assignRandomBits(generator, 4);
        // 16384  = 00000000000000000100000000000000
        // 147464 = 00000000000000100100000000001000
        REQUIRE(individual.ciag[0] == 16384);
        REQUIRE(individual.ciag[1] == 147464);
    }

    Individual individual;
    individual.ciag[0] = 16384;
    individual.ciag[1] = 147464;

    SECTION("ageing passing 0")
    {
        individual.ageByOneYear(17);
        REQUIRE(individual.ilosc_1 == 0);
    }

    SECTION("ageing passing 1")
    {
        individual.ageByOneYear(18);
        REQUIRE(individual.ilosc_1 == 1);
    }

    SECTION("ageing passing 0 after 1")
    {
        individual.ageByOneYear(18);
        individual.ageByOneYear(19);
        REQUIRE(individual.ilosc_1 == 1);
    }

    SECTION("ageing passing multiple 1s")
    {
        individual.ageByOneYear(18);
        individual.ageByOneYear(47);
        REQUIRE(individual.ilosc_1 == 2);
    }

    SECTION("ageing passing all bits")
    {
        for (unsigned int i{0}; i < Config::bits_; ++i)
            individual.ageByOneYear(i);
        REQUIRE(individual.ilosc_1 == 4);
    }

    SECTION("bits as string")
    {
        REQUIRE_THAT(
            individual.asBitString(),
            Catch::Matchers::Equals("000000000000000001000000000000000000000000"
                                    "0000100100000000001000"));
    }
}
