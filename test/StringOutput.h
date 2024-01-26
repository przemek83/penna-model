#pragma once

#include <Output.h>

#include <map>

class StringOutput : public Output
{
public:
    StringOutput();

protected:
    std::shared_ptr<std::ostream> openFile(OUTPUT_FILE file) const override;

private:
    std::map<OUTPUT_FILE, std::shared_ptr<std::ostringstream>> stringStreams_{
        {STATISTICS, std::make_shared<std::ostringstream>()},
        {INITIAL_POPULATION, std::make_shared<std::ostringstream>()},
        {FINAL_POPULATION, std::make_shared<std::ostringstream>()},
        {AGE_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {BITS_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {DEATHS_DISTRIBUTION, std::make_shared<std::ostringstream>()},
        {FAMILIES, std::make_shared<std::ostringstream>()}};
};
