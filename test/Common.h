#pragma once

#include <string>

namespace Common
{
std::string getFileContent(const std::string& file);

void compareFileWithExpected(const std::string& file);

void compareStringWithFileContent(const std::string& currentContent,
                                  const std::string& expectedFileName);
}  // namespace Common
