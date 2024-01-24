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

void compareFiles(const std::string& currentFile,
                  const std::string& expectedFile)
{
    const std::string currentContent{getFileContent(currentFile)};
    const std::string expectedContent{getFileContent(expectedFile)};
    CAPTURE(currentFile);
    REQUIRE_THAT(currentContent, Catch::Matchers::Equals(expectedContent));
}
}  // namespace Common
