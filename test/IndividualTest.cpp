#include <bitset>
#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "Individual.h"
#include "MockedGenerator.h"

TEST_CASE("Individual", "[penna]")
{
    MockedGenerator generator;
    SECTION("bit strings creation")
    {
        Individual individual;
        individual.assignRandomBits(generator, 4);
        // 16384  = 00000000000000000100000000000000
        // 147464 = 00000000000000100100000000001000
        REQUIRE(individual.ciag[0] == 16384);
        REQUIRE(individual.ciag[1] == 147464);
    }

    SECTION("ageing passing 0")
    {
        Individual individual;
        individual.assignRandomBits(generator, 4);
        individual.ageByOneYear(17);
        REQUIRE(individual.ilosc_1 == 0);
    }

    SECTION("ageing passing 1")
    {
        Individual individual;
        individual.assignRandomBits(generator, 4);
        individual.ageByOneYear(18);
        REQUIRE(individual.ilosc_1 == 1);
    }

    SECTION("ageing passing 0 after 1")
    {
        Individual individual;
        individual.assignRandomBits(generator, 4);
        individual.ageByOneYear(18);
        individual.ageByOneYear(19);
        REQUIRE(individual.ilosc_1 == 1);
    }

    SECTION("ageing passing multiple 1s")
    {
        Individual individual;
        individual.assignRandomBits(generator, 4);
        individual.ageByOneYear(18);
        individual.ageByOneYear(47);
        REQUIRE(individual.ilosc_1 == 2);
    }
}
