#include "Matrix.h"


Matrix::Matrix(
  const MatrixConfig& config
)
  : _config(config),
    _display(
      WIDTH,
      HEIGHT,
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

  clear();
  show();
}


void Matrix::clear() {
  _display.fillScreen(0);
}


void Matrix::drawPixel(
  Position position,
  Color color
) {
  const Position displayPosition =
    toDisplayPosition(position);

  _display.drawPixel(
    displayPosition.x,
    displayPosition.y,
    toDisplayColor(color)
  );
}


void Matrix::show() {
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


Adafruit_NeoMatrix& Matrix::display() {
  return _display;
}


Position Matrix::toDisplayPosition(
  Position position
) const {
  return {
    .x = position.x,
    .y = HEIGHT - 1 - position.y
  };
}


uint16_t Matrix::toDisplayColor(
  Color color
) {
  return _display.Color(
    color.r,
    color.g,
    color.b
  );
}


void Matrix::applyBrightness() {
  const uint8_t hardwareBrightness =
    static_cast<uint8_t>(
      (_brightness * 255)
      / MAX_BRIGHTNESS
    );

  _display.setBrightness(
    hardwareBrightness
  );
}