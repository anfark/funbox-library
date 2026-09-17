#include "Matrix.h"

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

Matrix::Matrix(const MatrixConfig& config)
  : _config(config),
    _display(
      MATRIX_WIDTH,
      MATRIX_HEIGHT,
      config.dataPin
    ) {}

void Matrix::setup() {
  _display.begin();

  _display.setBrightness(
    _config.brightness
  );

  _display.clear();
  _display.show();
}

Adafruit_NeoMatrix& Matrix::display() {
  return _display;
}