#include "FunBox.h"

#include <Arduino.h>


FunBox::FunBox()
  : FunBox(DEFAULT_FUNBOX_CONFIG) {
}


FunBox::FunBox(const FunBoxConfig& config)
  : _stick(config.stick),
    _matrix(config.matrix),
    _screen(config.screen),
    _audio(config.audio),
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
      _screen,
      _stick,
      _matrix,
      _audio
    ) {
}


void FunBox::setup() {
  _stick.setup();
  _matrix.setup();
  _screen.setup();
  _audio.setup();
  _settings.setup();

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


bool FunBox::addGame(Game& game) {
  if (_gameCount >= MAX_GAMES) {
    return false;
  }

  _games[_gameCount] = &game;
  ++_gameCount;

  _mainMenu.setGames(
    _games,
    _gameCount
  );

  return true;
}


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


bool FunBox::processSystemInput() {
  const bool setPressed =
    _stick.isPressed(StickKey::SET);

  const bool resetPressed =
    _stick.isPressed(StickKey::RESET);

  const bool setPressedNow =
    setPressed && !_setWasPressed;

  const bool resetPressedNow =
    resetPressed && !_resetWasPressed;

  _setWasPressed = setPressed;
  _resetWasPressed = resetPressed;

  if (resetPressedNow) {
    enterBoot();
    return true;
  }

  if (setPressedNow) {
    if (_state == FunBoxState::Settings) {
      closeSettings();
    } else {
      enterSettings();
    }

    return true;
  }

  return false;
}


void FunBox::updateBoot() {
  _bootSequence.update();

  if (_bootSequence.finished()) {
    enterMainMenu();
  }
}


void FunBox::updateMainMenu() {
  _mainMenu.update();

  if (!_mainMenu.hasSelection()) {
    return;
  }

  Game* game =
    _mainMenu.selectedGame();

  if (game != nullptr) {
    enterGame(*game);
  }
}


void FunBox::updateGame() {
  if (_activeGame == nullptr) {
    enterMainMenu();
    return;
  }

  _activeGame->update(*this);
}


void FunBox::updateSettings() {
  _settings.update();
}


void FunBox::enterBoot() {
  stopActiveGame();

  _state = FunBoxState::Boot;

  _audio.stop();

  _bootSequence.reset();
  _bootSequence.start();
}


void FunBox::enterMainMenu() {
  _state = FunBoxState::MainMenu;

  _mainMenu.start();
}


void FunBox::enterGame(Game& game) {
  _activeGame = &game;
  _state = FunBoxState::Game;

  _activeGame->start(*this);
}


void FunBox::enterSettings() {
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

      auto& display =
        _screen.display();

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


void FunBox::stopActiveGame() {
  if (_activeGame == nullptr) {
    return;
  }

  _activeGame->stop(*this);
  _activeGame = nullptr;
}