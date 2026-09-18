#pragma once

#include <cstddef>
#include <new>
#include <stdint.h>

#include "core/Color.h"
#include "core/Direction.h"
#include "core/Position.h"
#include "core/Size.h"

#include "hardware/Audio.h"
#include "hardware/Matrix.h"
#include "hardware/Screen.h"
#include "hardware/Stick.h"

#include "runtime/BaseGame.h"
#include "runtime/BootSequence.h"
#include "runtime/Game.h"
#include "runtime/GameDevices.h"
#include "runtime/GameRegistration.h"
#include "runtime/GameRender.h"
#include "runtime/GameUpdate.h"
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
    .reset = 21
  },

  .matrix = {
    .dataPin = 4,
    .brightness = 2
  },

  .screen = {
    .sdaPin = 8,
    .sclPin = 9
  },

  .audio = {
    .bclkPin = 6,
    .lrcPin = 5,
    .doutPin = 7,
    .volume = 3
  }
};


enum class FunBoxState {
  Boot,
  MainMenu,
  Game,
  GameOver,
  Settings
};


class FunBox {
public:
  FunBox();

  explicit FunBox(
    const FunBoxConfig& config
  );


  void setup();
  void update();


  template<typename Definition>
  bool addGame() {
    using Instance =
      Game<Definition>;

    static_assert(
      sizeof(Instance) <=
        GAME_STORAGE_SIZE,
      "Game is too large for FunBox game storage"
    );

    static_assert(
      alignof(Instance) <=
        alignof(std::max_align_t),
      "Game requires unsupported alignment"
    );

    if (
      _gameCount >=
      MAX_GAMES
    ) {
      return false;
    }

    _games[_gameCount] = {
      .name =
        Definition::NAME,

      .create =
        [](
          void* storage,
          GameDevices devices
        ) -> BaseGame* {
          return new (storage)
            Instance(devices);
        },

      .destroy =
        [](BaseGame* game) {
          static_cast<Instance*>(
            game
          )->~Instance();
        }
    };

    ++_gameCount;

    _mainMenu.setGames(
      _games,
      _gameCount
    );

    return true;
  }


  GameDevices devices();

  Stick& stick();
  Matrix& matrix();
  Screen& screen();
  Audio& audio();


private:
  static constexpr uint8_t MAX_GAMES = 8;

  static constexpr size_t GAME_STORAGE_SIZE =
    2048;

  static constexpr uint32_t TICKS_PER_SECOND =
    4;

  static constexpr uint32_t TICK_INTERVAL_MS =
    1000 / TICKS_PER_SECOND;


  FunBoxState _state =
    FunBoxState::Boot;

  FunBoxState _stateBeforeSettings =
    FunBoxState::MainMenu;


  Stick _stick;
  Matrix _matrix;
  Screen _screen;
  Audio _audio;


  BootSequence _bootSequence;
  MainMenu _mainMenu;
  Settings _settings;


  GameRegistration _games[
    MAX_GAMES
  ] = {};

  uint8_t _gameCount = 0;


  alignas(std::max_align_t)
  unsigned char _gameStorage[
    GAME_STORAGE_SIZE
  ] = {};


  const GameRegistration*
    _activeRegistration =
      nullptr;

  BaseGame* _activeGame =
    nullptr;


  uint32_t _lastGameTick = 0;


  bool processSystemInput();


  void updateBoot();
  void updateMainMenu();
  void updateGame();
  void updateGameOver();
  void updateSettings();


  void enterBoot();
  void enterMainMenu();

  void enterGame(
    const GameRegistration& game
  );

  void enterGameOver();

  void enterSettings();
  void closeSettings();

  void stopActiveGame();
};