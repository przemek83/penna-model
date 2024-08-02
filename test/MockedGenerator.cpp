#include "MockedGenerator.h"

#include <cmath>

MockedGenerator::MockedGenerator(int bits) : Generator(bits) {}

int MockedGenerator::getInt(int low, int high)
{
    // Following linear congruential generator algorithm described in
    // https://thompsonsed.co.uk/random-number-generators-for-c-performance-tested

    x_ = (multiplier_ * x_ + increment_) % modulus_;
    return low + x_ % (high - low + 1);
}

void MockedGenerator::setX(uint64_t x) { x_ = x; }
