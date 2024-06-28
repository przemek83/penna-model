#pragma once

#include <cstdint>

#include <src/Generator.h>

class MockedGenerator : public Generator
{
public:
    explicit MockedGenerator(int bits);

    int getInt(int low, int high) override;

    void setX(uint64_t x);

private:
    unsigned long int x_{12};
};
