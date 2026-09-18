#pragma once

#include <stdint.h>


enum class NoteDuration : uint8_t {
  Whole,
  Half,
  Quarter,
  Eighth
};


struct Note {
  float frequency;
  NoteDuration duration;
};


namespace Notes {

inline constexpr float REST = 0.0f;


// Oktave 4

inline constexpr float C4  = 261.63f;
inline constexpr float CS4 = 277.18f;
inline constexpr float D4  = 293.66f;
inline constexpr float DS4 = 311.13f;
inline constexpr float E4  = 329.63f;
inline constexpr float F4  = 349.23f;
inline constexpr float FS4 = 369.99f;
inline constexpr float G4  = 392.00f;
inline constexpr float GS4 = 415.30f;
inline constexpr float A4  = 440.00f;
inline constexpr float AS4 = 466.16f;
inline constexpr float B4  = 493.88f;


// Oktave 5

inline constexpr float C5  = 523.25f;
inline constexpr float CS5 = 554.37f;
inline constexpr float D5  = 587.33f;
inline constexpr float DS5 = 622.25f;
inline constexpr float E5  = 659.25f;
inline constexpr float F5  = 698.46f;
inline constexpr float FS5 = 739.99f;
inline constexpr float G5  = 783.99f;
inline constexpr float GS5 = 830.61f;
inline constexpr float A5  = 880.00f;
inline constexpr float AS5 = 932.33f;
inline constexpr float B5  = 987.77f;

}


constexpr uint32_t durationMs(
  NoteDuration duration,
  uint16_t bpm = 120
) {
  const uint32_t quarter =
    60000UL / bpm;

  switch (duration) {
    case NoteDuration::Whole:
      return quarter * 4;

    case NoteDuration::Half:
      return quarter * 2;

    case NoteDuration::Quarter:
      return quarter;

    case NoteDuration::Eighth:
      return quarter / 2;
  }

  return quarter;
}