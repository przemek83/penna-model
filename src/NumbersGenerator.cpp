#include "NumbersGenerator.h"

NumbersGenerator::NumbersGenerator(int bits, unsigned long seed)
    : Generator(bits), engine_{seed}
{
}

int NumbersGenerator::getInt(int low, int high)
{
    std::uniform_int_distribution<> dist{low, high};
    return dist(engine_);
}
