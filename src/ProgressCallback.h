#pragma once

#include <functional>

#include "Config.h"

namespace progress_callback
{
[[maybe_unused]] std::function<void(int)> getSequentialProgressCallback(
    int sim, const config::Params& params);

[[maybe_unused]] std::function<void(int)> getOverallProgressCallback(
    int sim, const config::Params& params);

int getLineLength();

}  // namespace progress_callback
