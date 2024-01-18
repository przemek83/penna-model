#pragma once

#include <cstdint>
#include "Generator.h"

class MockedGenerator : public Generator
{
public:
    int getInt(int low, int high) override;

private:
    uint64_t x_{12};
};
