#pragma once

#include <chrono>

class Timer
{
public:
    Timer() = default;
    ~Timer();

    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

private:
    std::chrono::time_point<std::chrono::steady_clock> start_{
        std::chrono::steady_clock::now()};
};
