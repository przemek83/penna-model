#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <src/Metrics.h>

TEST_CASE("Metrics", "[penna]")
{
    SECTION("getLivingAtEnd calculations")
    {
        metrics::BasicMetrics<int> metrics{100, 10, 5, 7};
        REQUIRE(getLivingAtEnd(metrics) == 8);
    }

    SECTION("serializeLifeRelatedData use")
    {
        metrics::BasicMetrics<int> metrics{100, 10, 5, 3};
        std::ostringstream stream;
        serializeLifeRelatedData(metrics, stream, ',');
        REQUIRE(stream.str() == "10,5,12,3");
    }

    SECTION("serializeFamily use")
    {
        metrics::BasicMetrics<int> metrics{4, 210, 320, 125};
        std::ostringstream stream;
        serializeFamily(metrics, stream);
        REQUIRE(stream.str() == "4");
    }
}
