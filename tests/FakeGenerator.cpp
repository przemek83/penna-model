#include "FakeGenerator.h"

FakeGenerator::FakeGenerator(std::list<int> numbers)
    : Generator{0}, numbers_{std::move(numbers)}
{
}

int FakeGenerator::getInt([[maybe_unused]] int low, [[maybe_unused]] int high)
{
    const int number{numbers_.front()};
    numbers_.pop_front();
    return number;
}
