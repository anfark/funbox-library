#pragma once

#include "hardware/Audio.h"
#include "hardware/Matrix.h"
#include "hardware/Screen.h"


struct GameDevices {
  Matrix& matrix;
  Screen& screen;
  Audio& audio;
};