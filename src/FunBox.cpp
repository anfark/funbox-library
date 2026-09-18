#include "FunBox.h"

#include <Arduino.h>


// Erzeugt eine FunBox mit der Standardkonfiguration.
FunBox::FunBox()
  : FunBox(
      DEFAULT_FUNBOX_CONFIG
    ) {
}


// Erzeugt eine FunBox mit der angegebenen Konfiguration.
FunBox::FunBox(
  const FunBoxConfig& config
)
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


// Initialisiert Hardware und startet die Bootsequenz.
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


// Aktualisiert Hardware und den aktuellen FunBox-Zustand.
void FunBox::update() {
  _stick.update();
  _audio.update();

  if (
    processSystemInput()
  ) {
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


// Bündelt die Spielhardware in einer gemeinsamen Struktur.
GameDevices FunBox::devices() {
  return {
    .matrix = _matrix,
    .screen = _screen,
    .audio = _audio
  };
}


// Liefert Zugriff auf den Stick.
Stick& FunBox::stick() {
  return _stick;
}


// Liefert Zugriff auf die Matrix.
Matrix& FunBox::matrix() {
  return _matrix;
}


// Liefert Zugriff auf das Display.
Screen& FunBox::screen() {
  return _screen;
}


// Liefert Zugriff auf die Audioausgabe.
Audio& FunBox::audio() {
  return _audio;
}


// Verarbeitet globale Systemtasten.
bool FunBox::processSystemInput() {
  for (
    const auto& event :
    _stick.events()
  ) {
    if (!event.isPressed) {
      continue;
    }

    if (
      event.key ==
      StickKey::RESET
    ) {
      enterBoot();
      return true;
    }

    if (
      event.key ==
      StickKey::SET
    ) {
      if (
        _state ==
        FunBoxState::Settings
      ) {
        closeSettings();
      }
      else {
        enterSettings();
      }

      return true;
    }
  }

  return false;
}


// Aktualisiert die Bootsequenz.
void FunBox::updateBoot() {
  _bootSequence.update();

  if (
    _bootSequence.finished()
  ) {
    enterMainMenu();
  }
}


// Aktualisiert das Hauptmenü und startet eine Auswahl.
void FunBox::updateMainMenu() {
  _mainMenu.update();

  if (
    !_mainMenu.hasSelection()
  ) {
    return;
  }

  const GameRegistration* game =
    _mainMenu.selectedGame();

  if (
    game != nullptr
  ) {
    enterGame(
      *game
    );
  }
}


// Aktualisiert Eingaben und Ticks des aktiven Spiels.
void FunBox::updateGame() {
  if (
    _activeGame ==
    nullptr
  ) {
    enterMainMenu();
    return;
  }


  for (
    const auto& event :
    _stick.events()
  ) {
    if (!event.isPressed) {
      continue;
    }

    switch (event.key) {
      case StickKey::UP:
        _activeGame->move(
          Direction::Up
        );
        break;

      case StickKey::DOWN:
        _activeGame->move(
          Direction::Down
        );
        break;

      case StickKey::LEFT:
        _activeGame->move(
          Direction::Left
        );
        break;

      case StickKey::RIGHT:
        _activeGame->move(
          Direction::Right
        );
        break;

      case StickKey::MID:
        _activeGame->click();
        break;

      case StickKey::SET:
      case StickKey::RESET:
      case StickKey::COUNT:
        break;
    }

    if (
      _activeGame->isOver()
    ) {
      enterGameOver();
      return;
    }
  }


  const uint32_t now =
    millis();

  if (
    now - _lastGameTick <
    TICK_INTERVAL_MS
  ) {
    return;
  }

  _lastGameTick = now;

  _activeGame->tick();

  if (
    _activeGame->isOver()
  ) {
    enterGameOver();
  }
}


// Wartet nach Game Over auf MID.
void FunBox::updateGameOver() {
  if (
    _activeGame ==
    nullptr
  ) {
    enterMainMenu();
    return;
  }

  for (
    const auto& event :
    _stick.events()
  ) {
    if (
      event.isPressed &&
      event.key ==
        StickKey::MID
    ) {
      enterMainMenu();
      return;
    }
  }
}


// Aktualisiert das Einstellungsmenü.
void FunBox::updateSettings() {
  _settings.update();
}


// Wechselt zur Bootsequenz.
void FunBox::enterBoot() {
  stopActiveGame();

  _state =
    FunBoxState::Boot;

  _audio.stop();

  _bootSequence.reset();
  _bootSequence.start();
}


// Wechselt zurück in das Hauptmenü.
void FunBox::enterMainMenu() {
  stopActiveGame();

  _audio.stop();

  _matrix.clear();
  _matrix.show();

  _state =
    FunBoxState::MainMenu;

  _mainMenu.start();
}


// Erzeugt und startet ein registriertes Spiel.
void FunBox::enterGame(
  const GameRegistration& registration
) {
  stopActiveGame();

  _activeRegistration =
    &registration;

  _activeGame =
    registration.create(
      _gameStorage,
      devices()
    );

  _lastGameTick =
    millis();

  _state =
    FunBoxState::Game;

  _activeGame->render();
}


// Wechselt in den terminalen Game-Over-Zustand.
void FunBox::enterGameOver() {
  _state =
    FunBoxState::GameOver;

  if (
    _activeGame !=
    nullptr
  ) {
    _activeGame->renderGameOver();
  }
}


// Öffnet das Einstellungsmenü.
void FunBox::enterSettings() {
  if (
    _state ==
    FunBoxState::Boot
  ) {
    return;
  }

  _stateBeforeSettings =
    _state;

  _state =
    FunBoxState::Settings;

  _settings.start();
}


// Schließt die Einstellungen und stellt den vorherigen Zustand wieder her.
void FunBox::closeSettings() {
  switch (
    _stateBeforeSettings
  ) {
    case FunBoxState::MainMenu:
      enterMainMenu();
      break;


    case FunBoxState::Game:
      _state =
        FunBoxState::Game;

      _lastGameTick =
        millis();

      if (
        _activeGame !=
        nullptr
      ) {
        _activeGame->render();
      }

      break;


    case FunBoxState::GameOver:
      _state =
        FunBoxState::GameOver;

      if (
        _activeGame !=
        nullptr
      ) {
        _activeGame->renderGameOver();
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


// Zerstört die aktuell laufende Spielinstanz.
void FunBox::stopActiveGame() {
  if (
    _activeGame != nullptr &&
    _activeRegistration != nullptr
  ) {
    _activeRegistration->destroy(
      _activeGame
    );
  }

  _activeGame = nullptr;
  _activeRegistration = nullptr;
}