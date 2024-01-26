#pragma once

#include <string>

namespace Common
{
std::string getFileContent(const std::string& file);

void compareFiles(const std::string& currentFile,
                  const std::string& expectedFile);

void compareStringWithFileContent(const std::string& currentContent,
                                  const std::string& expectedFileName);
}  // namespace Common
