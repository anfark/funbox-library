#pragma once

#include "core/Direction.h"
#include "core/Size.h"

struct Position {
    int x;
    int y;

    bool operator==(const Position&) const = default;
    static const Position Zero;

    static constexpr Position offset(Direction direction) {
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

    constexpr bool isInside(Size bounds) const {
        return x >= 0 &&
               y >= 0 &&
               x < bounds.w &&
               y < bounds.h;
    }

    constexpr bool isOutside(Size bounds) const {
        return !isInside(bounds);
    }
};

inline const Position Position::Zero{0, 0};

constexpr Position operator+(Position lhs, Position rhs) {
    return {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
}

constexpr Position operator%(Position pos, Size size) {
    return {
        .x = (pos.x % size.w + size.w) % size.w,
        .y = (pos.y % size.h + size.h) % size.h
    };
}



constexpr Position operator+(Position position, Direction direction) {
    return position + Position::offset(direction);
}

constexpr Position& operator+=(Position& position, Direction direction) {
    position = position + direction;
    return position;
}