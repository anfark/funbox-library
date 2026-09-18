#pragma once

#include <algorithm>
#include <compare>
#include <vector>

#include "Direction.h"
#include "Size.h"


struct Position {
  int x;
  int y;

  static const Position Zero;


  bool operator==(
    const Position&
  ) const = default;

  auto operator<=>(
    const Position&
  ) const = default;


  // Prüft, ob die Position innerhalb der angegebenen Größe liegt.
  constexpr bool isInside(
    const Size& bounds
  ) const {
    return
      x >= 0 &&
      y >= 0 &&
      x < bounds.w &&
      y < bounds.h;
  }


  // Prüft, ob die Position außerhalb der angegebenen Größe liegt.
  constexpr bool isOutside(
    const Size& bounds
  ) const {
    return !isInside(
      bounds
    );
  }


  // Liefert den Positions-Offset für die angegebene Richtung.
  static constexpr Position offset(
    Direction direction
  ) {
    switch (direction) {
      case Direction::Up:
        return {
          0,
          1
        };

      case Direction::Down:
        return {
          0,
          -1
        };

      case Direction::Left:
        return {
          -1,
          0
        };

      case Direction::Right:
        return {
          1,
          0
        };
    }

    return Zero;
  }
};


inline const Position Position::Zero{
  0,
  0
};


// Addiert zwei Positionen.
constexpr Position operator+(
  Position lhs,
  Position rhs
) {
  return {
    .x = lhs.x + rhs.x,
    .y = lhs.y + rhs.y
  };
}


// Addiert einen Richtungs-Offset auf eine Position.
constexpr Position operator+(
  Position position,
  Direction direction
) {
  return
    position +
    Position::offset(direction);
}


// Begrenzt eine Position zyklisch auf die angegebene Größe.
constexpr Position operator%(
  Position position,
  Size size
) {
  return {
    .x =
      (
        position.x % size.w +
        size.w
      ) % size.w,

    .y =
      (
        position.y % size.h +
        size.h
      ) % size.h
  };
}


// Prüft, ob eine Positionsliste die angegebene Position enthält.
inline bool contains(
  const std::vector<Position>& positions,
  Position position
) {
  return std::find(
    positions.begin(),
    positions.end(),
    position
  ) != positions.end();
}