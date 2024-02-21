#include "Generator.h"

#include "Config.h"

int Generator::getPercentChance() { return getInt(0, 100); }

int Generator::getBitPosition() { return getInt(0, Config::Params::bits_ - 1); }
