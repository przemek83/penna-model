#pragma once

#include <src/Output.h>

#include <map>
#include <sstream>

class StringOutput : public Output
{
public:
    StringOutput() = default;

    std::string getContentForOutputType(OutputType outputType);

    void reset();

protected:
    std::shared_ptr<std::ostream> getStream(
        OutputType outputType) const override;

private:
    std::map<OutputType, std::shared_ptr<std::ostringstream>> stringStreams_{
        {OutputType::STATISTICS, std::make_shared<std::ostringstream>()},
        {OutputType::INITIAL_POPULATION,
         std::make_shared<std::ostringstream>()},
        {OutputType::FINAL_POPULATION, std::make_shared<std::ostringstream>()},
        {OutputType::AGE_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {OutputType::BITS_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {OutputType::DEATHS_DISTRIBUTION,
         std::make_shared<std::ostringstream>()},
        {OutputType::FAMILIES, std::make_shared<std::ostringstream>()}};
};
