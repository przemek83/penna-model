#include "Generator.h"

#include "Config.h"

Generator::Generator(int bits) : bits_(bits) {}

int Generator::getPercentChance() { return getInt(0, 100); }

int Generator::getBitPosition() { return getInt(0, bits_ - 1); }
