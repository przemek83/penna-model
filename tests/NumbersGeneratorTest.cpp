#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <src/NumbersGenerator.h>

TEST_CASE("NumbersGenerator", "[penna]")
{
    const int seed{7};
    NumbersGenerator generator{8, seed};

    SECTION("Get bit position")
    {
        for (auto i : {0UL, 1UL, 6UL, 2UL, 3UL, 7UL, 5UL, 3UL})
            REQUIRE(generator.getBitPosition() == i);
    }

    SECTION("Get percent chance")
    {
        for (auto i : {7, 22, 78, 32, 44, 98, 73, 46})
            REQUIRE(generator.getPercentChance() == i);
    }

    SECTION("Get int")
    {
        for (auto i :
             {7630, 22'734, 77'992, 31'897, 43'841, 97'823, 72'347, 45'558})
            REQUIRE(generator.getInt(0, 100'000) == i);
    }

    SECTION("Get int for 0 generator")
    {
        for (auto i : {0, 0, 0})
            REQUIRE(generator.getInt(0, 0) == i);
    }

    SECTION("Get int for single nmumber generator")
    {
        const int expected{66};
        for (auto i : {expected, expected, expected})
            REQUIRE(generator.getInt(expected, expected) == i);
    }
}
