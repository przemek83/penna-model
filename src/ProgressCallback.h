#pragma once

#include <functional>

#include "Config.h"

namespace ProgressCallback
{
[[maybe_unused]] std::function<void(int)> createSequentialProgressCallback(
    int sim, const Config::Params& params);

[[maybe_unused]] std::function<void(int)> createOverallProgressCallback(
    int sim, const Config::Params& params);
}  // namespace ProgressCallback
