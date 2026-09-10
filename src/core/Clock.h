#pragma once

#include <cstdint>
#include "core/TimeSource.h"
#include "core/Tick.h"

class ClockListener {
public:
    virtual ~ClockListener() = default;

    virtual void onTick(const Tick& tick) = 0;
};

class Clock {
public:
  Clock(
    TimeSource& timeSource,
    ClockListener& listener,
    uint64_t interval
  );

  void update();
private:
  TimeSource& _timeSource;
  ClockListener& _listener;
  uint64_t _interval;
  uint64_t _lastTick;
};