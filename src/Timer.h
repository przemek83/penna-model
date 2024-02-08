#pragma once

#include <chrono>
#include <ostream>

class Timer
{
public:
    ~Timer();

private:
    std::chrono::time_point<std::chrono::steady_clock> start_{
        std::chrono::steady_clock::now()};
};
