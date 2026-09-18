#pragma once

#include "BaseGame.h"
#include "GameDevices.h"


struct GameRegistration {
  const char* name;

  BaseGame* (*create)(
    void* storage,
    GameDevices devices
  );

  void (*destroy)(
    BaseGame* game
  );
};