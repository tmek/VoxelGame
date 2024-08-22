// copyright

#pragma once

#include <atomic>

// alias TAtomic for std::atomic
template <typename T>
struct TAtomic : public std::atomic<T>
{
};
