#include "Screen.h"

#include <Arduino.h>
#include <Wire.h>


Screen::Screen(
  const ScreenConfig& config
)
  : _config(config),
    _display(
      128,
      64,
      &Wire,
      -1
    ) {
}


// Initialisiert das OLED-Display und leert den Bildschirm.
void Screen::setup() {
  Wire.begin(
    _config.sdaPin,
    _config.sclPin
  );

  _display.begin(
    SSD1306_SWITCHCAPVCC,
    0x3C
  );

  _display.clearDisplay();

  _display.setTextColor(
    SSD1306_WHITE
  );

  _display.display();
}


// Liefert direkten Zugriff auf das zugrunde liegende Display.
Adafruit_SSD1306& Screen::display() {
  return _display;
}


// Zeichnet eine große Zahl zentriert auf das Display.
void Screen::drawNumber(
  uint32_t value
) {
  _display.clearDisplay();

  _display.setTextColor(
    SSD1306_WHITE
  );

  _display.setTextSize(5);

  const String text =
    String(value);

  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  _display.getTextBounds(
    text,
    0,
    0,
    &x1,
    &y1,
    &width,
    &height
  );

  _display.setCursor(
    (128 - width) / 2,
    (64 - height) / 2
  );

  _display.print(
    text
  );

  _display.display();
}


// Zeichnet einen Game-Over-Bildschirm mit dem angegebenen Score.
void Screen::drawGameOver(
  uint32_t score
) {
  _display.clearDisplay();

  _display.setTextColor(
    SSD1306_WHITE
  );

  _display.setTextSize(2);

  _display.setCursor(
    10,
    8
  );

  _display.println(
    "GAME OVER"
  );

  _display.setTextSize(1);

  _display.setCursor(
    38,
    40
  );

  _display.print(
    "Score: "
  );

  _display.println(
    score
  );

  _display.display();
}