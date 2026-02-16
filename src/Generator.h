#pragma once

#include <cstddef>

class Generator
{
public:
    explicit Generator(int bits);
    Generator(const Generator&) = delete;
    Generator(Generator&&) = delete;

    virtual ~Generator() = default;

    Generator& operator=(const Generator&) = delete;
    Generator& operator=(Generator&&) = delete;

    int getPercentChance();

    std::size_t getBitPosition();

protected:
    virtual int getInt(int low, int high) = 0;

private:
    int bits_;
};
