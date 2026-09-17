#pragma once

#include <stdint.h>

#include <Adafruit_SSD1306.h>

struct ScreenConfig {
  uint8_t sdaPin;
  uint8_t sclPin;
};

class Screen {
public:
  explicit Screen(const ScreenConfig& config);

  void setup();

  Adafruit_SSD1306& display();

private:
  ScreenConfig _config;
  Adafruit_SSD1306 _display;
};