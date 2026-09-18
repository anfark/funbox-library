#pragma once

#include <stdint.h>

#include "hardware/Audio.h"
#include "hardware/Matrix.h"
#include "hardware/Screen.h"


class BootSequence {
public:
  BootSequence(
    Screen& screen,
    Matrix& matrix,
    Audio& audio
  );

  void start();
  void update();
  void reset();

  bool finished() const;

private:
  Screen& _screen;
  Matrix& _matrix;
  Audio& _audio;

  bool _started = false;
  bool _finished = false;

  uint32_t _startedAt = 0;

  void drawScreen();
  void drawWave(
    uint32_t elapsed,
    uint32_t duration
  );
};