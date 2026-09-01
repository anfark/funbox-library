#include <gtest/gtest.h>

#include "Clock.h"
#include "FakeTimeSource.h"

class TestListener : public ClockListener {
public:
    void onTick(const Tick& tick) {
        tickCount++;
        lastTick = tick;
    }

    int tickCount = 0;
    Tick lastTick{};
};

TEST(ClockTest, DoesNotTickImmediately) {
    FakeTimeSource time;
    TestListener listener;

    Clock clock(time, listener, 1000);
    clock.update();

    EXPECT_EQ(listener.tickCount, 0);
}

TEST(ClockTest, TicksAfterInterval) {
    FakeTimeSource time;
    TestListener listener;

    Clock clock(time, listener, 1000);

    time.advance(1000);
    clock.update();

    EXPECT_EQ(listener.tickCount, 1);
    EXPECT_EQ(listener.lastTick.timestamp, 1000);
}