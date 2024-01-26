#pragma once

#include <Output.h>

#include <map>
#include <sstream>

class StringOutput : public Output
{
public:
    StringOutput();

    std::string getContentForOutputType(OUTPUT_TYPE outputType);

protected:
    std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const override;

private:
    std::map<OUTPUT_TYPE, std::shared_ptr<std::ostringstream>> stringStreams_{
        {STATISTICS, std::make_shared<std::ostringstream>()},
        {INITIAL_POPULATION, std::make_shared<std::ostringstream>()},
        {FINAL_POPULATION, std::make_shared<std::ostringstream>()},
        {AGE_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {BITS_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {DEATHS_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {FAMILIES, std::make_shared<std::ostringstream>()}};
};
