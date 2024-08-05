#include "Generator.h"

#include "Config.h"

Generator::Generator(int bits) : bits_(bits) {}

int Generator::getPercentChance() { return getInt(0, config::maxPercentage); }

std::size_t Generator::getBitPosition()
{
    return static_cast<std::size_t>(getInt(0, bits_ - 1));
}
