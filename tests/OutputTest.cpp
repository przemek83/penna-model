#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <src/AvgResults.h>
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

class TestRunListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(
        [[maybe_unused]] Catch::TestRunInfo const& testRunInfo) override
    {
        config::Params params;
        params.population_.max_ = maxPopulation;
        params.years_ = years;

        const int simId{0};
        Simulation simulation1(params, simId);
        simulation1.setGenerator(
            common::getTestGenerator(config::Params::bits_));
        simulation1.createInitialPopulation();
        simulation1.saveInitialPopulation(output);
        const SimResults data1{simulation1.run()};

        simulation1.saveFinalPopulation(output);
        output.saveSimulationData(data1);
    }
};

CATCH_REGISTER_LISTENER(TestRunListener)

TEST_CASE("Output", "[penna]")
{
    auto outputType{
        GENERATE(OutputType::STATISTICS, OutputType::INITIAL_POPULATION,
                 OutputType::FINAL_POPULATION, OutputType::AGE_DISTRIBUTION,
                 OutputType::BITS_DISTRIBUTION, OutputType::DEATHS_DISTRIBUTION,
                 OutputType::FAMILIES)};
    CAPTURE(outputType);
    const FileOutput fileOutput("sim1");
    common::compareStringWithFileContent(
        output.getContentForOutputType(outputType),
        fileOutput.getName(outputType));
}

TEST_CASE("Output averages", "[penna]")
{
    const int simId{0};

    SECTION("save averages")
    {
        config::Params params;
        params.population_.max_ = maxPopulation;
        params.years_ = years;

        Simulation simulation1(params, simId);
        simulation1.setGenerator(
            common::getTestGenerator(config::Params::bits_));
        simulation1.createInitialPopulation();
        const SimResults data1{simulation1.run()};

        auto generator{
            std::make_unique<MockedGenerator>(config::Params::bits_)};
        generator->setX(14'947'405'050'666'326'135ULL);
        Simulation simulation2(params, simId);
        simulation2.setGenerator(std::move(generator));
        simulation2.createInitialPopulation();
        const SimResults data2{simulation2.run()};

        const AvgResults averageData{{data1, data2},
                                     static_cast<std::size_t>(params.years_),
                                     config::Params::bits_};

        output.reset();
        output.saveAverages(averageData);

        const std::vector<OutputType> outputTypes{
            OutputType::DEATHS_DISTRIBUTION, OutputType::FAMILIES,
            OutputType::BITS_DISTRIBUTION, OutputType::AGE_DISTRIBUTION,
            OutputType::STATISTICS};

        const FileOutput fileOutput("averages");
        for (const auto outputType : outputTypes)
        {
            CAPTURE(outputType);
            common::compareStringWithFileContent(
                output.getContentForOutputType(outputType),
                fileOutput.getName(outputType));
        }
    }
}
