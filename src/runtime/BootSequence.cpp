#include "BootSequence.h"


namespace {

const Melody BOOT_MELODY = {
  {
    Notes::C5,
    NoteDuration::Eighth
  },
  {
    Notes::E5,
    NoteDuration::Eighth
  },
  {
    Notes::G5,
    NoteDuration::Eighth
  },
  {
    Notes::C5,
    NoteDuration::Quarter
  }
};


constexpr uint8_t WAVE_WIDTH = 3;

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
  if (_started) {
    return;
  }

  _started = true;
  _finished = false;
  _startedAt = millis();

  drawScreen();

  _audio.effect(
    BOOT_MELODY
  );
}


void BootSequence::update() {
  if (
    !_started ||
    _finished
  ) {
    return;
  }

  const uint32_t elapsed =
    millis() - _startedAt;

  const uint32_t duration =
    durationMs(
      BOOT_MELODY
    );

  drawWave(
    elapsed,
    duration
  );

  if (elapsed >= duration) {
    _matrix.clear();
    _matrix.show();

    _finished = true;
  }
}


void BootSequence::reset() {
  _started = false;
  _finished = false;
  _startedAt = 0;

  _matrix.clear();
  _matrix.show();
}


bool BootSequence::finished() const {
  return _finished;
}


void BootSequence::drawScreen() {
  auto& display =
    _screen.display();

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(
    SSD1306_WHITE
  );

  display.setCursor(
    25,
    24
  );

  display.println(
    "FUNBOX"
  );

  display.display();
}


void BootSequence::drawWave(
  uint32_t elapsed,
  uint32_t duration
) {
  _matrix.clear();

  constexpr int LAST_DIAGONAL =
    Matrix::WIDTH +
    Matrix::HEIGHT -
    2;

  constexpr int TRAVEL_DISTANCE =
    LAST_DIAGONAL +
    WAVE_WIDTH;

  const float progress =
    duration == 0
      ? 1.0f
      : static_cast<float>(elapsed) /
        static_cast<float>(duration);

  const float waveFront =
    progress *
    TRAVEL_DISTANCE;

  for (
    int y = 0;
    y < Matrix::HEIGHT;
    ++y
  ) {
    for (
      int x = 0;
      x < Matrix::WIDTH;
      ++x
    ) {
      const int diagonal =
        x +
        (Matrix::HEIGHT - 1 - y);

      const float distance =
        waveFront -
        diagonal;

      if (
        distance < 0 ||
        distance >= WAVE_WIDTH
      ) {
        continue;
      }

      const int band =
        static_cast<int>(
          distance
        );

      Color color;

      switch (band) {
        case 0:
          color = Color::Red;
          break;

        case 1:
          color = Color::Green;
          break;

        default:
          color = Color::Blue;
          break;
      }

      _matrix.drawPixel(
        Position{
          .x = x,
          .y = y
        },
        color
      );
    }
  }

  _matrix.show();
}