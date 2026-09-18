#pragma once

#include <stdint.h>

#include "GameRegistration.h"
#include "hardware/Screen.h"
#include "hardware/Stick.h"


class MainMenu {
public:
  MainMenu(
    Screen& screen,
    Stick& stick
  );

  void start();
  void update();

  void setGames(
    const GameRegistration* games,
    uint8_t gameCount
  );

  bool hasSelection() const;

  const GameRegistration*
  selectedGame() const;


private:
  Screen& _screen;
  Stick& _stick;

  const GameRegistration* _games =
    nullptr;

  uint8_t _gameCount = 0;
  uint8_t _selectedIndex = 0;

  bool _selectionReady = false;


  void draw();
  void handleInput();
};