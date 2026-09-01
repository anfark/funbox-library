#pragma once

#include <cstdint>

class TimeSource {
public:
    virtual ~TimeSource() = default;
    virtual uint64_t now() const = 0;
};