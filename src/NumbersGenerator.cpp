#include "NumbersGenerator.h"

NumbersGenerator::NumbersGenerator() : engine_{std::random_device()()} {}

int NumbersGenerator::getInt(int low, int high)
{
    std::uniform_int_distribution<> dist{low, high};
    return dist(engine_);
}
