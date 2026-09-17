#pragma once

#include "snake/Direction.h"

struct Position {
    int x;
    int y;

    bool operator==(const Position&) const = default;
    static const Position Zero;
};

inline const Position Position::Zero{0, 0};

constexpr Position operator+(Position lhs, Position rhs) {
    return {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
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

constexpr Position& operator+=(Position& position, Direction direction) {
    position = position + direction;
    return position;
}