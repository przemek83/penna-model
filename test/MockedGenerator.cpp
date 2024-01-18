#include "MockedGenerator.h"

int MockedGenerator::getInt(int low, int high)
{
    // Following SplitMix64 algorithm described in
    // https://thompsonsed.co.uk/random-number-generators-for-c-performance-tested
    uint64_t z = (x_ += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return low + ((z ^ (z >> 31)) % (high - low + 1));
}
