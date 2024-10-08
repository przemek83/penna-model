#include "Common.h"

#include <fstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <src/Generator.h>

#include "MockedGenerator.h"

namespace common
{
std::string getFileContent(const std::string& file)
{
    std::ifstream ifstream(file);
    std::string content{(std::istreambuf_iterator<char>(ifstream)),
                        (std::istreambuf_iterator<char>())};
    return content;
}

void compareStringWithFileContent(const std::string& currentContent,
                                  const std::string& expectedFileName)
{
    const std::string expectedContent{
        getFileContent("TestFiles/" + expectedFileName)};
    CAPTURE(expectedFileName);
    REQUIRE_THAT(currentContent, Catch::Matchers::Equals(expectedContent));
}

std::unique_ptr<Generator> getTestGenerator(int bits)
{
    return std::make_unique<MockedGenerator>(bits);
}

}  // namespace common
