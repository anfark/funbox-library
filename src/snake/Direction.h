#pragma once

#include "snake/Position.h"

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

constexpr Position offset(Direction direction) {
    switch (direction) {
        case Direction::Up:
            return {0, 1};

        case Direction::Down:
            return {0, -1};

        case Direction::Left:
            return {-1, 0};

        case Direction::Right:
            return {1, 0};
    }

    return Position::Zero;
}

constexpr Position operator+(Position position, Direction direction) {
    return position + offset(direction);
}