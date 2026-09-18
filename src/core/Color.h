#pragma once

#include <stdint.h>


struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  bool operator==(const Color&) const = default;


  static const Color Black;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color White;
};


inline const Color Color::Black{
  0,
  0,
  0
};

inline const Color Color::Red{
  255,
  0,
  0
};

inline const Color Color::Green{
  0,
  255,
  0
};

inline const Color Color::Blue{
  0,
  0,
  255
};

inline const Color Color::White{
  255,
  255,
  255
};