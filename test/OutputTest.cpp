#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <src/AverageData.h>
#include <src/FileOutput.h>
#include <src/Simulation.h>

#include "Common.h"
#include "MockedGenerator.h"
#include "StringOutput.h"

namespace
{
const int years{1000};
const int maxPopulation{5000};
StringOutput output;
}  // namespace

class testRunListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(
        [[maybe_unused]] Catch::TestRunInfo const& testRunInfo) override
    {
        Config::Params params;
        params.population_.max_ = maxPopulation;
        params.years_ = years;

        Simulation simulation1(params);
        simulation1.setGenerator(Common::getTestGenerator(params.bits_));
        simulation1.createInitialPopulation();
        simulation1.saveInitialPopulation(output);
        const SimulationData data1{simulation1.run()};

        simulation1.saveFinalPopulation(output);
        output.saveSimulationData(data1);
    }
};

CATCH_REGISTER_LISTENER(testRunListener)

TEST_CASE("Output", "[penna]")
{
    auto outputType{GENERATE(STATISTICS, INITIAL_POPULATION, FINAL_POPULATION,
                             AGE_DISTRIBUTION, BITS_DISTRIBUTION,
                             DEATHS_DISTRIBUTION, FAMILIES)};
    CAPTURE(outputType);
    const FileOutput fileOutput("sim1");
    Common::compareStringWithFileContent(
        output.getContentForOutputType(outputType),
        fileOutput.getName(outputType));
}

TEST_CASE("Output averages", "[penna]")
{
    SECTION("save averages")
    {
        Config::Params params;
        params.population_.max_ = maxPopulation;
        params.years_ = years;

        Simulation simulation1(params);
        simulation1.setGenerator(Common::getTestGenerator(params.bits_));
        simulation1.createInitialPopulation();
        const SimulationData data1{simulation1.run()};

        auto generator{std::make_unique<MockedGenerator>(params.bits_)};
        generator->setX(14947405050666326135ULL);
        Simulation simulation2(params);
        simulation2.setGenerator(std::move(generator));
        simulation2.createInitialPopulation();
        const SimulationData data2{simulation2.run()};

        const AverageData averageData{{data1, data2},
                                      static_cast<std::size_t>(params.years_),
                                      params.bits_};

        output.reset();
        output.saveAverages(averageData);

        const std::vector<OUTPUT_TYPE> outputTypes{
            DEATHS_DISTRIBUTION, FAMILIES, BITS_DISTRIBUTION, AGE_DISTRIBUTION,
            STATISTICS};

        const FileOutput fileOutput("averages");
        for (const auto outputType : outputTypes)
        {
            CAPTURE(outputType);
            Common::compareStringWithFileContent(
                output.getContentForOutputType(outputType),
                fileOutput.getName(outputType));
        }
    }
}
