#pragma once

#include "Generator.h"

#include <random>

class NumbersGenerator : public Generator
{
public:
    NumbersGenerator(int bits, unsigned long seed);

    int getInt(int low, int high) override;

private:
    std::mt19937 engine_;
};
