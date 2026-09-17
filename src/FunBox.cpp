#include "FunBox.h"


FunBox::FunBox()
  : _stick({
      .up = 0,
      .down = 1,
      .left = 2,
      .right = 3,
      .mid = 10,
      .set = 20,
      .reset = 21,
    }),

    _matrix({
      .dataPin = 4,
      .brightness = 8,
    }),

    _screen({
      .sdaPin = 8,
      .sclPin = 9,
    }),

    _audio({
      .bclkPin = 6,
      .lrcPin = 5,
      .doutPin = 7,
    }),

    _bootSequence(
      _screen,
      _matrix,
      _audio
    ),

    _mainMenu(
      _screen,
      _stick
    ),

    _settings(
      _screen
    ) {
}


// ==================================================
// Setup / Update
// ==================================================

void FunBox::setup() {
  _stick.setup();
  _matrix.setup();
  _screen.setup();
  _audio.setup();

  _mainMenu.setGames(
    _games,
    _gameCount
  );

  _setWasPressed =
    _stick.isPressed(StickKey::SET);

  _resetWasPressed =
    _stick.isPressed(StickKey::RESET);

  enterBoot();
}


void FunBox::update() {
  _stick.update();
  _audio.update();

  if (processSystemInput()) {
    return;
  }

  switch (_state) {
    case FunBoxState::Boot:
      updateBoot();
      break;

    case FunBoxState::MainMenu:
      updateMainMenu();
      break;

    case FunBoxState::Game:
      updateGame();
      break;

    case FunBoxState::Settings:
      updateSettings();
      break;
  }
}


// ==================================================
// Games
// ==================================================

bool FunBox::addGame(Game& game) {
  if (_gameCount >= MAX_GAMES) {
    return false;
  }

  _games[_gameCount] = &game;
  _gameCount++;

  _mainMenu.setGames(
    _games,
    _gameCount
  );

  return true;
}


void FunBox::updateGame() {
  if (_activeGame == nullptr) {
    enterMainMenu();
    return;
  }

  _activeGame->update(*this);
}


void FunBox::enterGame(Game& game) {
  _activeGame = &game;
  _state = FunBoxState::Game;

  _activeGame->start(*this);
}


void FunBox::stopActiveGame() {
  if (_activeGame == nullptr) {
    return;
  }

  _activeGame->stop(*this);
  _activeGame = nullptr;
}


// ==================================================
// System input
// ==================================================

bool FunBox::processSystemInput() {
  const bool setPressed =
    _stick.isPressed(StickKey::SET);

  const bool resetPressed =
    _stick.isPressed(StickKey::RESET);

  const bool setJustPressed =
    setPressed && !_setWasPressed;

  const bool resetJustPressed =
    resetPressed && !_resetWasPressed;

  _setWasPressed = setPressed;
  _resetWasPressed = resetPressed;


  // RESET hat immer Vorrang.
  if (resetJustPressed) {
    enterBoot();
    return true;
  }


  if (setJustPressed) {
    if (_state == FunBoxState::Settings) {
      closeSettings();
    } else {
      enterSettings();
    }

    return true;
  }


  return false;
}


// ==================================================
// Boot
// ==================================================

void FunBox::updateBoot() {
  _bootSequence.update();

  if (_bootSequence.finished()) {
    enterMainMenu();
  }
}


void FunBox::enterBoot() {
  stopActiveGame();

  _state = FunBoxState::Boot;

  _audio.stop();

  _bootSequence.start();
}


// ==================================================
// Main menu
// ==================================================

void FunBox::updateMainMenu() {
  _mainMenu.update();

  if (!_mainMenu.hasSelection()) {
    return;
  }

  Game* game = _mainMenu.selectedGame();

  if (game == nullptr) {
    return;
  }

  enterGame(*game);
}


void FunBox::enterMainMenu() {
  _state = FunBoxState::MainMenu;

  _mainMenu.start();
}


// ==================================================
// Settings
// ==================================================

void FunBox::updateSettings() {
  _settings.update();
}


void FunBox::enterSettings() {
  // Settings waehrend der Bootsequenz machen wenig Sinn.
  if (_state == FunBoxState::Boot) {
    return;
  }

  _stateBeforeSettings = _state;
  _state = FunBoxState::Settings;

  _settings.start();
}


void FunBox::closeSettings() {
  switch (_stateBeforeSettings) {
    case FunBoxState::MainMenu:
      enterMainMenu();
      break;

    case FunBoxState::Game: {
      _state = FunBoxState::Game;
    
      auto& display = _screen.display();
    
      display.clearDisplay();
      display.display();
    
      break;
    }

    case FunBoxState::Boot:
      enterBoot();
      break;

    case FunBoxState::Settings:
      enterMainMenu();
      break;
  }
}


// ==================================================
// Hardware
// ==================================================

Stick& FunBox::stick() {
  return _stick;
}


Matrix& FunBox::matrix() {
  return _matrix;
}


Screen& FunBox::screen() {
  return _screen;
}


Audio& FunBox::audio() {
  return _audio;
}