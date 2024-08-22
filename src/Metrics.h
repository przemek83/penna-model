#pragma once

#include <ostream>

namespace metrics
{
template <typename T>
struct BasicMetrics
{
    T families_{0};
    T livingAtStart_{0};
    T births_{0};
    T deaths_{0};
};

template <typename T>
auto getLivingAtEnd(T metrics)
{
    return (metrics.livingAtStart_ - metrics.deaths_) + metrics.births_;
}

template <typename T>
void serializeLifeRelatedData(T metrics, std::ostream& stream, char separator)
{
    stream << metrics.livingAtStart_ << separator << metrics.births_
           << separator << getLivingAtEnd<T>(metrics) << separator
           << metrics.deaths_;
}

template <typename T>
void serializeFamily(T metrics, std::ostream& stream)
{
    stream << metrics.families_;
}
};  // namespace metrics
