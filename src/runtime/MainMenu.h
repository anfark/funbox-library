#pragma once

#include <stdint.h>

#include "hardware/Screen.h"
#include "hardware/Stick.h"
#include "runtime/GameDescription.h"


class MainMenu {
public:
  MainMenu(
    Screen& screen,
    Stick& stick
  );

  void start();
  void update();

  void setGames(
    GameDescription* const* games,
    uint8_t gameCount
  );

  bool hasSelection() const;

  GameDescription* selectedGame() const;


private:
  Screen& _screen;
  Stick& _stick;

  GameDescription* const* _games = nullptr;
  uint8_t _gameCount = 0;

  uint8_t _selectedIndex = 0;

  bool _selectionReady = false;

  void draw();
  void handleInput();
};