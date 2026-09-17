#pragma once

#include <stdint.h>

#include "hardware/Audio.h"
#include "hardware/Matrix.h"
#include "hardware/Screen.h"
#include "hardware/Stick.h"

#include "runtime/BootSequence.h"
#include "runtime/Game.h"
#include "runtime/MainMenu.h"
#include "runtime/Settings.h"


struct FunBoxConfig {
  StickPin stick;
  MatrixConfig matrix;
  ScreenConfig screen;
  AudioConfig audio;
};


inline constexpr FunBoxConfig DEFAULT_FUNBOX_CONFIG = {
  .stick = {
    .up = 0,
    .down = 1,
    .left = 2,
    .right = 3,
    .mid = 10,
    .set = 20,
    .reset = 21,
  },
  .matrix = {
    .dataPin = 4,
    .brightness = 2,
  },
  .screen = {
    .sdaPin = 8,
    .sclPin = 9,
  },
  .audio = {
    .bclkPin = 6,
    .lrcPin = 5,
    .doutPin = 7,
    .volume = 3,
  },
};


enum class FunBoxState {
  Boot,
  MainMenu,
  Game,
  Settings
};


class FunBox {
public:
  FunBox();
  explicit FunBox(const FunBoxConfig& config);

  void setup();
  void update();

  bool addGame(Game& game);

  Stick& stick();
  Matrix& matrix();
  Screen& screen();
  Audio& audio();

private:
  static constexpr uint8_t MAX_GAMES = 8;

  FunBoxState _state = FunBoxState::Boot;
  FunBoxState _stateBeforeSettings = FunBoxState::MainMenu;

  Stick _stick;
  Matrix _matrix;
  Screen _screen;
  Audio _audio;

  BootSequence _bootSequence;
  MainMenu _mainMenu;
  Settings _settings;

  Game* _games[MAX_GAMES] = {};
  uint8_t _gameCount = 0;

  Game* _activeGame = nullptr;

  bool _setWasPressed = false;
  bool _resetWasPressed = false;

  bool processSystemInput();

  void updateBoot();
  void updateMainMenu();
  void updateGame();
  void updateSettings();

  void enterBoot();
  void enterMainMenu();
  void enterGame(Game& game);
  void enterSettings();
  void closeSettings();

  void stopActiveGame();
};