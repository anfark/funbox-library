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
      .brightness = 2,
    }),

    _screen({
      .sdaPin = 8,
      .sclPin = 9,
    }),

    _audio({
      .bclkPin = 6,
      .lrcPin = 5,
      .doutPin = 7,
      .volume = 3,
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
      _screen,
      _stick,
      _matrix,
      _audio
    ) {
}


// ==================================================
// Setup / Update
// ==================================================

void FunBox::setup() {
  Serial.println();
  Serial.println("=== FunBox setup ===");

  Serial.println("[1] Stick setup...");
  _stick.setup();
  Serial.println("[1] Stick OK");

  Serial.println("[2] Matrix setup...");
  _matrix.setup();
  Serial.println("[2] Matrix OK");

  Serial.println("[3] Screen setup...");
  _screen.setup();
  Serial.println("[3] Screen OK");

  Serial.println("[4] Audio setup...");
  _audio.setup();
  Serial.println("[4] Audio OK");

  Serial.println("[5] Settings setup...");
  _settings.setup();
  Serial.println("[5] Settings OK");

  Serial.print("    Brightness: ");
  Serial.println(_matrix.brightness());

  Serial.print("    Volume: ");
  Serial.println(_audio.volume());

  Serial.println("[6] MainMenu setup...");

  _mainMenu.setGames(
    _games,
    _gameCount
  );

  Serial.print("    Games: ");
  Serial.println(_gameCount);

  Serial.println("[6] MainMenu OK");

  _setWasPressed =
    _stick.isPressed(StickKey::SET);

  _resetWasPressed =
    _stick.isPressed(StickKey::RESET);

  Serial.print("SET pressed at startup: ");
  Serial.println(_setWasPressed ? "yes" : "no");

  Serial.print("RESET pressed at startup: ");
  Serial.println(_resetWasPressed ? "yes" : "no");

  Serial.println("[7] Enter boot...");
  enterBoot();
  Serial.println("[7] Boot started");

  Serial.println("=== FunBox setup finished ===");
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
  Serial.print("Adding game: ");
  Serial.println(game.name());

  if (_gameCount >= MAX_GAMES) {
    Serial.println("ERROR: Maximum game count reached");
    return false;
  }

  _games[_gameCount] = &game;
  _gameCount++;

  _mainMenu.setGames(
    _games,
    _gameCount
  );

  Serial.print("Game count: ");
  Serial.println(_gameCount);

  return true;
}


void FunBox::updateGame() {
  if (_activeGame == nullptr) {
    Serial.println("ERROR: Game state without active game");
    enterMainMenu();
    return;
  }

  _activeGame->update(*this);
}


void FunBox::enterGame(Game& game) {
  Serial.print("Entering game: ");
  Serial.println(game.name());

  _activeGame = &game;
  _state = FunBoxState::Game;

  _activeGame->start(*this);
}


void FunBox::stopActiveGame() {
  if (_activeGame == nullptr) {
    return;
  }

  Serial.print("Stopping game: ");
  Serial.println(_activeGame->name());

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


  if (resetJustPressed) {
    Serial.println("RESET pressed -> Boot");
    enterBoot();
    return true;
  }


  if (setJustPressed) {
    Serial.println("SET pressed");

    if (_state == FunBoxState::Settings) {
      Serial.println("Closing settings");
      closeSettings();
    } else {
      Serial.println("Opening settings");
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
    Serial.println("Boot sequence finished -> MainMenu");
    enterMainMenu();
  }
}


void FunBox::enterBoot() {
  Serial.println("enterBoot()");

  stopActiveGame();

  _state = FunBoxState::Boot;

  Serial.println("Stopping audio...");
  _audio.stop();

  Serial.println("Starting boot sequence...");
  _bootSequence.start();

  Serial.println("Boot sequence started");
}


// ==================================================
// Main menu
// ==================================================

void FunBox::updateMainMenu() {
  _mainMenu.update();

  if (!_mainMenu.hasSelection()) {
    return;
  }

  Game* game =
    _mainMenu.selectedGame();

  if (game == nullptr) {
    Serial.println("ERROR: MainMenu selected null game");
    return;
  }

  enterGame(*game);
}


void FunBox::enterMainMenu() {
  Serial.println("enterMainMenu()");

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
  if (_state == FunBoxState::Boot) {
    Serial.println("Ignoring settings during boot");
    return;
  }

  Serial.println("enterSettings()");

  _stateBeforeSettings = _state;
  _state = FunBoxState::Settings;

  _settings.start();
}


void FunBox::closeSettings() {
  Serial.println("closeSettings()");

  switch (_stateBeforeSettings) {
    case FunBoxState::MainMenu:
      Serial.println("Return to MainMenu");
      enterMainMenu();
      break;

    case FunBoxState::Game: {
      Serial.println("Return to Game");

      _state = FunBoxState::Game;

      auto& display = _screen.display();

      display.clearDisplay();
      display.display();

      break;
    }

    case FunBoxState::Boot:
      Serial.println("Return to Boot");
      enterBoot();
      break;

    case FunBoxState::Settings:
      Serial.println("Unexpected Settings -> Settings state");
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