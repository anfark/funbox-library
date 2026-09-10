#pragma once

enum class Direction {
    Up    = 0,
    Left  = 1,
    Down  = 2,
    Right = 3
};

constexpr Direction inverseDirection(Direction direction) {
    return static_cast<Direction>(
        (static_cast<int>(direction) + 2) % 4
    );
}