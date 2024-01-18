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
        REQUIRE(individual.ciag[0] == 16384);
        REQUIRE(individual.ciag[1] == 147464);
    }
}
