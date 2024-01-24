#pragma once

#include <string>

namespace Common
{
std::string getFileContent(const std::string& file);

void compareFiles(const std::string& currentFile,
                  const std::string& expectedFile);
}  // namespace Common
