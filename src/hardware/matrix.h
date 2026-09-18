#pragma once

#include <stdint.h>

#include <Adafruit_NeoMatrix.h>

#include "core/Color.h"
#include "core/Position.h"
#include "core/Size.h"


struct MatrixConfig {
  uint8_t dataPin;
  uint8_t brightness;
};


class Matrix {
public:
  static constexpr uint8_t WIDTH = 8;
  static constexpr uint8_t HEIGHT = 8;

  static constexpr Size SIZE = {
    .w = WIDTH,
    .h = HEIGHT
  };


  explicit Matrix(
    const MatrixConfig& config
  );


  void setup();


  void clear();

  void drawPixel(
    Position position,
    Color color
  );

  void show();


  void brightness(uint8_t value);
  uint8_t brightness() const;


  Adafruit_NeoMatrix& display();


private:
  static constexpr uint8_t MAX_BRIGHTNESS = 8;

  MatrixConfig _config;
  Adafruit_NeoMatrix _display;

  uint8_t _brightness;


  Position toDisplayPosition(
    Position position
  ) const;

  uint16_t toDisplayColor(
    Color color
  );

  void applyBrightness();
};