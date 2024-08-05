#pragma once

#include <iostream>

class Logger
{
public:
    void log(const std::string& message) const
    {
        std::cout << message;
        std::cout.flush();
    }

    void err(const std::string& errorMessage) const
    {
        std::cerr << errorMessage;
        std::cerr.flush();
    }
};
