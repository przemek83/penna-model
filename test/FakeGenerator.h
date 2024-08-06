#pragma once

#include <cstdint>
#include <list>

#include <src/Generator.h>

class FakeGenerator : public Generator
{
public:
    explicit FakeGenerator(std::list<int> numbers);

    int getInt(int low, int high) override;

private:
    std::list<int> numbers_;
};
