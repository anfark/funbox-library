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

  Adafruit_NeoMatrix& display();

private:
  MatrixConfig _config;
  Adafruit_NeoMatrix _display;
};