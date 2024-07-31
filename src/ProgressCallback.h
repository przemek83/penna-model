#pragma once

#include <functional>

#include "Config.h"

namespace ProgressCallback
{
[[maybe_unused]] std::function<void(int)> getSequentialProgressCallback(
    int sim, const Config::Params& params);

[[maybe_unused]] std::function<void(int)> getOverallProgressCallback(
    int sim, const Config::Params& params);
}  // namespace ProgressCallback
