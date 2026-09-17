#include "Matrix.h"

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8


Matrix::Matrix(const MatrixConfig& config)
  : _config(config),
    _display(
      MATRIX_WIDTH,
      MATRIX_HEIGHT,
      config.dataPin
    ),
    _brightness(config.brightness) {

  if (_brightness > MAX_BRIGHTNESS) {
    _brightness = MAX_BRIGHTNESS;
  }
}


void Matrix::setup() {
  _display.begin();

  applyBrightness();

  _display.clear();
  _display.show();
}


void Matrix::brightness(uint8_t value) {
  if (value > MAX_BRIGHTNESS) {
    value = MAX_BRIGHTNESS;
  }

  _brightness = value;

  applyBrightness();

  // NeoPixel brightness wird erst mit show()
  // auf die LEDs übertragen.
  _display.show();
}


uint8_t Matrix::brightness() const {
  return _brightness;
}


void Matrix::applyBrightness() {
  const uint8_t hardwareBrightness =
    static_cast<uint8_t>(
      (_brightness * 255) / MAX_BRIGHTNESS
    );

  _display.setBrightness(
    hardwareBrightness
  );
}


Adafruit_NeoMatrix& Matrix::display() {
  return _display;
}