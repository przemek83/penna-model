#include "Common.h"

#include <fstream>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers_string.hpp"

namespace Common
{
std::string getFileContent(const std::string& file)
{
    std::ifstream ifstream(file);
    std::string content{(std::istreambuf_iterator<char>(ifstream)),
                        (std::istreambuf_iterator<char>())};
    return content;
}

void compareFileWithExpected(const std::string& file)
{
    const std::string currentContent{getFileContent(file)};
    const std::string expectedContent{getFileContent("TestFiles/" + file)};
    CAPTURE(file);
    REQUIRE_THAT(currentContent, Catch::Matchers::Equals(expectedContent));
}

void compareStringWithFileContent(const std::string& currentContent,
                                  const std::string& expectedFileName)
{
    const std::string expectedContent{
        getFileContent("TestFiles/" + expectedFileName)};
    CAPTURE(expectedFileName);
    REQUIRE_THAT(currentContent, Catch::Matchers::Equals(expectedContent));
}

}  // namespace Common
