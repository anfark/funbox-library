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

  unsigned long _startedAt = 0;
  unsigned long _lastFrameAt = 0;
  unsigned long _lastToneAt = 0;

  uint8_t _wavePosition = 0;
  uint8_t _toneIndex = 0;

  bool _running = false;
  bool _finished = false;

  void drawScreen();
  void updateMatrix(unsigned long now);
  void updateAudio(unsigned long now);

  void finish();
};