#pragma once

#include <functional>

#include "Config.h"

namespace progressCallback
{
[[maybe_unused]] std::function<void(int)> getSequentialProgressCallback(
    int sim, const config::Params& params);

[[maybe_unused]] std::function<void(int)> getOverallProgressCallback(
    int sim, const config::Params& params);
}  // namespace progressCallback
