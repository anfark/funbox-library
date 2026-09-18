#pragma once

#include <stdint.h>

#include <Adafruit_SSD1306.h>


struct ScreenConfig {
  uint8_t sdaPin;
  uint8_t sclPin;
};


class Screen {
public:
  explicit Screen(
    const ScreenConfig& config
  );

  void setup();

  Adafruit_SSD1306& display();

  // Zeichnet eine große Zahl zentriert auf das Display.
  void drawNumber(
    uint32_t value
  );

  // Zeichnet einen Game-Over-Bildschirm mit dem angegebenen Score.
  void drawGameOver(
    uint32_t score
  );

private:
  ScreenConfig _config;

  Adafruit_SSD1306 _display;
};