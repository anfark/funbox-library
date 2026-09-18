#pragma once

#include <stdint.h>
#include <variant>
#include <vector>

#include "Note.h"


using Melody =
  std::vector<Note>;


struct Noise {
  NoteDuration duration;
};


using SoundEffect =
  std::variant<
    Melody,
    Noise
  >;


inline uint32_t durationMs(
  const Melody& melody,
  uint16_t bpm = 120
) {
  uint32_t total = 0;

  for (const auto& note : melody) {
    total += durationMs(
      note.duration,
      bpm
    );
  }

  return total;
}