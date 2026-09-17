#include "Screen.h"

#include <Arduino.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Screen::Screen(const ScreenConfig& config)
  : _config(config),
    _display(
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      &Wire,
      OLED_RESET
    ) {}

void Screen::setup() {
  Wire.begin(
    _config.sdaPin,
    _config.sclPin
  );

  if (!_display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      )) {
    Serial.println("OLED init failed");
    return;
  }

  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);
  _display.display();
}

Adafruit_SSD1306& Screen::display() {
  return _display;
}