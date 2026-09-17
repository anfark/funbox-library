#pragma once

#include "hardware/Audio.h"
#include "hardware/Matrix.h"
#include "hardware/Screen.h"
#include "hardware/Stick.h"

class FunBox {
public:
  FunBox();

  void setup();
  void update();

  Stick& stick();
  Matrix& matrix();
  Screen& screen();
  Audio& audio();

private:
  Stick _stick;
  Matrix _matrix;
  Screen _screen;
  Audio _audio;
};