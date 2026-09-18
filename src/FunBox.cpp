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

    case FunBoxState::GameOver:
      updateGameOver();
      break;

    case FunBoxState::Settings:
      updateSettings();
      break;
  }
}


bool FunBox::addGame(
  GameDescription& game
) {
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
  for (const auto& event : _stick.events()) {
    if (!event.isPressed) {
      continue;
    }

    if (event.key == StickKey::RESET) {
      enterBoot();
      return true;
    }

    if (event.key == StickKey::SET) {
      if (_state == FunBoxState::Settings) {
        closeSettings();
      } else {
        enterSettings();
      }

      return true;
    }
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

  GameDescription* game =
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

  for (const auto& event : _stick.events()) {
    if (!event.isPressed) {
      continue;
    }

    switch (event.key) {
      case StickKey::UP:
        _activeGame->move(Direction::Up);
        break;

      case StickKey::DOWN:
        _activeGame->move(Direction::Down);
        break;

      case StickKey::LEFT:
        _activeGame->move(Direction::Left);
        break;

      case StickKey::RIGHT:
        _activeGame->move(Direction::Right);
        break;

      case StickKey::MID:
        _activeGame->click();
        break;

      case StickKey::SET:
      case StickKey::RESET:
      case StickKey::COUNT:
        break;
    }

    if (_activeGame->isOver()) {
      enterGameOver();
      return;
    }
  }

  const uint32_t now = millis();

  if (
    now - _lastGameTick >= TICK_INTERVAL_MS
  ) {
    _lastGameTick = now;

    _activeGame->tick();

    if (_activeGame->isOver()) {
      enterGameOver();
      return;
    }
  }
}


void FunBox::updateGameOver() {
  if (_activeGame == nullptr) {
    enterMainMenu();
    return;
  }

  // Keine weiteren Actions oder Ticks.
  //
  // Der letzte vom Spiel gerenderte Zustand
  // bleibt dadurch erhalten.
  //
  // Hier können wir später zusätzlich einen
  // Game-Over-Bildschirm / Overlay behandeln.
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
  stopActiveGame();

  _state = FunBoxState::MainMenu;

  _mainMenu.start();
}


void FunBox::enterGame(
  GameDescription& description
) {
  _activeGame = &description.game();

  _activeGame->reset();

  _lastGameTick = millis();

  _state = FunBoxState::Game;

  _activeGame->render();
}


void FunBox::enterGameOver() {
  _state = FunBoxState::GameOver;
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

    case FunBoxState::Game:
      _state = FunBoxState::Game;

      // Die Zeit in den Settings zählt
      // nicht zur Spielzeit.
      _lastGameTick = millis();

      _activeGame->render();
      break;

    case FunBoxState::GameOver:
      _state = FunBoxState::GameOver;

      if (_activeGame != nullptr) {
        _activeGame->render();
      }

      break;

    case FunBoxState::Boot:
      enterBoot();
      break;

    case FunBoxState::Settings:
      enterMainMenu();
      break;
  }
}


void FunBox::stopActiveGame() {
  _activeGame = nullptr;
}