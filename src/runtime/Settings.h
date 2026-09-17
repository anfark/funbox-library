#pragma once

#include <stdint.h>

#include "hardware/Audio.h"
#include "hardware/Matrix.h"
#include "hardware/Screen.h"
#include "hardware/Stick.h"

class Settings {
public:
  Settings(
    Screen& screen,
    Stick& stick,
    Matrix& matrix,
    Audio& audio
  );

  void setup();
  void start();
  void update();

private:
  enum class Option : uint8_t {
    Brightness,
    Volume,
    Count
  };

  static constexpr uint8_t MIN_VALUE = 0;
  static constexpr uint8_t MAX_VALUE = 8;

  static constexpr uint8_t DEFAULT_BRIGHTNESS = 2;
  static constexpr uint8_t DEFAULT_VOLUME = 3;

  Screen& _screen;
  Stick& _stick;
  Matrix& _matrix;
  Audio& _audio;

  Option _selectedOption = Option::Brightness;

  uint8_t _brightness = DEFAULT_BRIGHTNESS;
  uint8_t _volume = DEFAULT_VOLUME;

  bool _upWasPressed = false;
  bool _downWasPressed = false;
  bool _leftWasPressed = false;
  bool _rightWasPressed = false;

  void load();
  void saveBrightness();
  void saveVolume();

  void handleInput();

  void selectPrevious();
  void selectNext();

  void decreaseValue();
  void increaseValue();

  void applyBrightness();
  void applyVolume();

  void draw();
};