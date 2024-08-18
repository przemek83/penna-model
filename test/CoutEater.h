#pragma once

#include <iostream>
#include <sstream>

class CoutEater
{
public:
    CoutEater();
    ~CoutEater();

    std::string getOutput() const;

private:
    std::streambuf* stdBuffer_{std::cout.rdbuf()};
    std::ostringstream fakeOutput_;
};
