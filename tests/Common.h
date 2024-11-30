#pragma once

#include <memory>
#include <string>

class Generator;

namespace common
{
std::string getFileContent(const std::string& file);

void compareStringWithFileContent(const std::string& currentContent,
                                  const std::string& expectedFileName);

std::unique_ptr<Generator> getTestGenerator(int bits);
}  // namespace common
