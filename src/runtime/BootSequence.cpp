#include "BootSequence.h"

#include <Arduino.h>

namespace {

constexpr unsigned long BOOT_DURATION_MS = 2200;
constexpr unsigned long FRAME_INTERVAL_MS = 90;
constexpr unsigned long TONE_INTERVAL_MS = 180;

constexpr uint8_t MATRIX_WIDTH = 8;
constexpr uint8_t MATRIX_HEIGHT = 8;

constexpr float BOOT_TONES[] = {
  440.0f,
  660.0f,
  880.0f,
  1320.0f
};

constexpr uint8_t BOOT_TONE_COUNT =
  sizeof(BOOT_TONES) / sizeof(BOOT_TONES[0]);

}

BootSequence::BootSequence(
  Screen& screen,
  Matrix& matrix,
  Audio& audio
)
  : _screen(screen),
    _matrix(matrix),
    _audio(audio) {
}

void BootSequence::start() {
  _startedAt = millis();
  _lastFrameAt = 0;
  _lastToneAt = 0;

  _wavePosition = 0;
  _toneIndex = 0;

  _running = true;
  _finished = false;

  drawScreen();

  _audio.tone(BOOT_TONES[0]);
}

void BootSequence::update() {
  if (!_running) {
    return;
  }

  const unsigned long now = millis();

  updateMatrix(now);
  updateAudio(now);

  if (now - _startedAt >= BOOT_DURATION_MS) {
    finish();
  }
}

void BootSequence::reset() {
  start();
}

bool BootSequence::finished() const {
  return _finished;
}

void BootSequence::drawScreen() {
  auto& display = _screen.display();

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(20, 16);
  display.println("FunBox");

  display.setTextSize(1);
  display.setCursor(40, 44);
  display.println("v0.1.0");

  display.display();
}

void BootSequence::updateMatrix(unsigned long now) {
  if (
    _lastFrameAt != 0 &&
    now - _lastFrameAt < FRAME_INTERVAL_MS
  ) {
    return;
  }

  _lastFrameAt = now;

  auto& display = _matrix.display();

  display.clear();

  for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
      const uint8_t diagonal = x + y;

      if (diagonal > _wavePosition) {
        continue;
      }

      const uint8_t phase =
        (_wavePosition - diagonal) % 6;

      uint16_t color;

      switch (phase) {
        case 0:
          color = display.Color(255, 0, 0);
          break;

        case 1:
          color = display.Color(255, 100, 0);
          break;

        case 2:
          color = display.Color(255, 255, 0);
          break;

        case 3:
          color = display.Color(0, 255, 0);
          break;

        case 4:
          color = display.Color(0, 120, 255);
          break;

        default:
          color = display.Color(180, 0, 255);
          break;
      }

      display.drawPixel(
        x,
        y,
        color
      );
    }
  }

  display.show();

  _wavePosition++;

  if (_wavePosition > 20) {
    _wavePosition = 0;
  }
}

void BootSequence::updateAudio(unsigned long now) {
  if (
    _lastToneAt != 0 &&
    now - _lastToneAt < TONE_INTERVAL_MS
  ) {
    return;
  }

  _lastToneAt = now;

  _audio.tone(
    BOOT_TONES[_toneIndex]
  );

  _toneIndex++;

  if (_toneIndex >= BOOT_TONE_COUNT) {
    _toneIndex = 0;
  }
}

void BootSequence::finish() {
  _running = false;
  _finished = true;

  _audio.stop();

  auto& matrix = _matrix.display();
  matrix.clear();
  matrix.show();
}