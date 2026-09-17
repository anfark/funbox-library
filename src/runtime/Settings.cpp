#include "Settings.h"

Settings::Settings(Screen& screen)
  : _screen(screen) {
}

void Settings::start() {
  draw();
}

void Settings::update() {
  // Spaeter:
  // - Lautstaerke
  // - Matrix-Helligkeit
  // - weitere Einstellungen
}

void Settings::draw() {
  auto& display = _screen.display();

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SETTINGS");

  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE
  );

  display.setCursor(0, 20);
  display.println("Coming soon");

  display.setCursor(0, 52);
  display.println("SET to close");

  display.display();
}