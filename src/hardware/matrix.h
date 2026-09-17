#pragma once

#include <stdint.h>

#include <Adafruit_NeoMatrix.h>

struct MatrixConfig {
  uint8_t dataPin, brightness;
};