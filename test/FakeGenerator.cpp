#include "FakeGenerator.h"

FakeGenerator::FakeGenerator(std::list<int> numbers)
    : Generator{0}, numbers_{numbers}
{
}

int FakeGenerator::getInt(int low, int high)
{
    const int number{numbers_.front()};
    numbers_.pop_front();
    return number;
}
