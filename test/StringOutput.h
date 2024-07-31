#pragma once

#include <src/Output.h>

#include <map>
#include <sstream>

class StringOutput : public Output
{
public:
    StringOutput() = default;

    std::string getContentForOutputType(OUTPUT_TYPE outputType);

    void reset();

protected:
    std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const override;

private:
    std::map<OUTPUT_TYPE, std::shared_ptr<std::ostringstream>> stringStreams_{
        {OUTPUT_TYPE::STATISTICS, std::make_shared<std::ostringstream>()},
        {OUTPUT_TYPE::INITIAL_POPULATION,
         std::make_shared<std::ostringstream>()},
        {OUTPUT_TYPE::FINAL_POPULATION, std::make_shared<std::ostringstream>()},
        {OUTPUT_TYPE::AGE_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {OUTPUT_TYPE::BITS_DISTRIBUTION,
         std::make_shared<std::ostringstream>()},
        {OUTPUT_TYPE::DEATHS_DISTRIBUTION,
         std::make_shared<std::ostringstream>()},
        {OUTPUT_TYPE::FAMILIES, std::make_shared<std::ostringstream>()}};
};
