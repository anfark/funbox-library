#pragma once

#include <stdint.h>

#include <Adafruit_NeoMatrix.h>

struct MatrixConfig {
  uint8_t dataPin;
  uint8_t brightness;
};

class Matrix {
public:
  explicit Matrix(const MatrixConfig& config);

  void setup();

  void brightness(uint8_t value);
  uint8_t brightness() const;

  Adafruit_NeoMatrix& display();

private:
  static constexpr uint8_t MAX_BRIGHTNESS = 8;

  MatrixConfig _config;
  Adafruit_NeoMatrix _display;

  uint8_t _brightness;

  void applyBrightness();
};