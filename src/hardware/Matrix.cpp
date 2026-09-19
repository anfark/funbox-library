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
    _brightness(
      config.brightness
    ) {

  if (
    _brightness >
    MAX_BRIGHTNESS
  ) {
    _brightness =
      MAX_BRIGHTNESS;
  }
}


// Initialisiert die LED-Matrix und leert alle Pixel.
void Matrix::setup() {
  _display.begin();

  applyBrightness();

  _display.setRotation(
    _config.rotation
  );

  clear();
  show();
}


// Löscht alle Pixel der Matrix.
void Matrix::clear() {
  _display.fillScreen(0);
}


// Zeichnet einen Pixel an der angegebenen Core-Position.
void Matrix::drawPixel(
  Position position,
  Color color
) {
  const Position displayPosition =
    toDisplayPosition(
      position
    );

  _display.drawPixel(
    displayPosition.x,
    displayPosition.y,
    toDisplayColor(
      color
    )
  );
}


// Zeichnet mehrere Pixel mit derselben Farbe.
void Matrix::drawPixels(
  const std::vector<Position>& positions,
  Color color
) {
  for (
    const auto& position :
    positions
  ) {
    drawPixel(
      position,
      color
    );
  }
}


// Überträgt den aktuellen Pixelzustand auf die LEDs.
void Matrix::show() {
  _display.show();
}


// Ändert die Helligkeit der Matrix.
void Matrix::brightness(
  uint8_t value
) {
  if (
    value >
    MAX_BRIGHTNESS
  ) {
    value =
      MAX_BRIGHTNESS;
  }

  _brightness =
    value;

  applyBrightness();

  _display.show();
}


// Liefert die aktuelle Helligkeit zurück.
uint8_t Matrix::brightness() const {
  return _brightness;
}


// Liefert direkten Zugriff auf die zugrunde liegende NeoMatrix.
Adafruit_NeoMatrix& Matrix::display() {
  return _display;
}


// Wandelt eine mathematische Core-Position in Display-Koordinaten um.
Position Matrix::toDisplayPosition(
  Position position
) const {
  return {
    .x = position.x,
    .y =
      HEIGHT - 1 - position.y
  };
}


// Wandelt eine Core-Farbe in das interne Matrixformat um.
uint16_t Matrix::toDisplayColor(
  Color color
) {
  return _display.Color(
    color.r,
    color.g,
    color.b
  );
}


// Überträgt die logische Helligkeit auf die NeoMatrix.
void Matrix::applyBrightness() {
  const uint8_t hardwareBrightness =
    static_cast<uint8_t>(
      (
        _brightness * 255
      )
      / MAX_BRIGHTNESS
    );

  _display.setBrightness(
    hardwareBrightness
  );
}