#pragma once

#include "TimeSource.h"

class FakeTimeSource : public TimeSource {
public:
    uint64_t now() const override {
        return _currentTime;
    }

    void advance(uint64_t millis) {
        _currentTime += millis;
    }
private:
    uint64_t _currentTime = 0;
};