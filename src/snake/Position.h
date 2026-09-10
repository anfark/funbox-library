#pragma once

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