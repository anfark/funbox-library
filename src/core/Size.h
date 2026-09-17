#pragma once

struct Size {
    int w;
    int h;

    bool operator==(const Size&) const = default;
    static const Size Zero;
};

inline const Size Size::Zero{0, 0};

constexpr Size operator+(Size lhs, Size rhs) {
    return {
        .w = lhs.w + rhs.w,
        .h = lhs.h + rhs.h
    };
}