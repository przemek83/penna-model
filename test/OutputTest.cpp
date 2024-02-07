#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include "Common.h"
#include "FileOutput.h"
#include "MockedGenerator.h"
#include "NullOutput.h"
#include "Simulation.h"
#include "StringOutput.h"

namespace
{
const int years{1000};
const int maxPopulation{5000};
StringOutput output(0, years, 0);
}  // namespace

class testRunListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(
        [[maybe_unused]] Catch::TestRunInfo const& testRunInfo) override
    {
        Config config;
        config.maxPopulation_ = maxPopulation;
        config.years_ = years;

        MockedGenerator generator;

        Simulation simulation1(config, 50);
        const SingleSimulationData data1{simulation1.run(generator, output)};
        std::cout << std::endl;
    }
};

CATCH_REGISTER_LISTENER(testRunListener)

TEST_CASE("Output", "[penna]")
{
    auto outputType{GENERATE(Output::STATISTICS, Output::INITIAL_POPULATION,
                             Output::FINAL_POPULATION, Output::AGE_DISTRIBUTION,
                             Output::BITS_DISTRIBUTION,
                             Output::DEATHS_DISTRIBUTION, Output::FAMILIES)};
    CAPTURE(outputType);
    const FileOutput fileOutput(0, years, 1);
    const std::string file{fileOutput.getName(outputType)};
    Common::compareStringWithFileContent(
        output.getContentForOutputType(outputType), file);
}

TEST_CASE("Output averages", "[penna]")
{
    SECTION("save averages")
    {
        Config config;
        config.maxPopulation_ = maxPopulation;
        config.years_ = years;

        MockedGenerator generator;
        NullOutput nullOutput;

        Simulation simulation1(config, 50);
        const SingleSimulationData data1{
            simulation1.run(generator, nullOutput)};
        std::cout << std::endl;

        Simulation simulation2(config, 50);
        const SingleSimulationData data2{
            simulation2.run(generator, nullOutput)};
        std::cout << std::endl;

        SimulationAverages simulationAverages{
            static_cast<std::size_t>(config.years_)};
        simulationAverages.integrateData(data1);
        simulationAverages.integrateData(data2);

        simulationAverages.finalize();

        output.reset();
        output.saveAverages(simulationAverages);

        const std::vector<Output::OUTPUT_TYPE> outputTypes{
            Output::DEATHS_DISTRIBUTION, Output::FAMILIES,
            Output::BITS_DISTRIBUTION, Output::AGE_DISTRIBUTION,
            Output::STATISTICS};

        const FileOutput fileOutput(0, years, 0);
        for (const auto outputType : outputTypes)
        {
            CAPTURE(outputType);
            Common::compareStringWithFileContent(
                output.getContentForOutputType(outputType),
                fileOutput.getName(outputType));
        }
    }
}
