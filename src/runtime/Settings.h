#pragma once

#include "hardware/Screen.h"

class Settings {
public:
  explicit Settings(Screen& screen);

  void start();
  void update();

private:
  Screen& _screen;

  void draw();
};