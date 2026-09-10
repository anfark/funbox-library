#include "core/Clock.h"

Clock::Clock(
    TimeSource& timeSource,
    ClockListener& listener,
    uint64_t interval
) : _timeSource(timeSource),
    _listener(listener),
    _interval(interval),
    _lastTick(timeSource.now()) {}

void Clock::update() {
    const auto now = _timeSource.now();

    if(now - _lastTick >= _interval) {
        _lastTick = now;
        _listener.onTick(Tick{
            .timestamp = now
        });
    }
}