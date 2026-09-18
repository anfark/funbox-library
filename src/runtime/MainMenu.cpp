#include "MainMenu.h"


MainMenu::MainMenu(
  Screen& screen,
  Stick& stick
)
  : _screen(screen),
    _stick(stick) {
}


void MainMenu::start() {
  _selectedIndex = 0;
  _selectionReady = false;

  draw();
}


void MainMenu::update() {
  handleInput();
}


void MainMenu::setGames(
  GameDescription* const* games,
  uint8_t gameCount
) {
  _games = games;
  _gameCount = gameCount;

  if (_selectedIndex >= _gameCount) {
    _selectedIndex = 0;
  }
}


bool MainMenu::hasSelection() const {
  return _selectionReady;
}


GameDescription* MainMenu::selectedGame() const {
  if (
    !_selectionReady ||
    _games == nullptr ||
    _gameCount == 0
  ) {
    return nullptr;
  }

  return _games[_selectedIndex];
}


void MainMenu::handleInput() {
  if (_gameCount == 0) {
    return;
  }

  if (_stick.isPressed(StickKey::UP)) {
    if (_selectedIndex == 0) {
      _selectedIndex = _gameCount - 1;
    } else {
      _selectedIndex--;
    }

    draw();
  }

  if (_stick.isPressed(StickKey::DOWN)) {
    _selectedIndex++;

    if (_selectedIndex >= _gameCount) {
      _selectedIndex = 0;
    }

    draw();
  }

  if (_stick.isPressed(StickKey::MID)) {
    _selectionReady = true;
  }
}


void MainMenu::draw() {
  auto& display = _screen.display();

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("FUNBOX");

  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE
  );

  if (_gameCount == 0) {
    display.setCursor(0, 20);
    display.println("No games installed");

    display.display();
    return;
  }

  constexpr uint8_t MAX_VISIBLE_GAMES = 5;

  uint8_t firstVisible = 0;

  if (_selectedIndex >= MAX_VISIBLE_GAMES) {
    firstVisible =
      _selectedIndex - MAX_VISIBLE_GAMES + 1;
  }

  for (
    uint8_t row = 0;
    row < MAX_VISIBLE_GAMES;
    row++
  ) {
    const uint8_t gameIndex =
      firstVisible + row;

    if (gameIndex >= _gameCount) {
      break;
    }

    display.setCursor(
      0,
      16 + row * 9
    );

    if (gameIndex == _selectedIndex) {
      display.print("> ");
    } else {
      display.print("  ");
    }

    display.println(
      _games[gameIndex]->name()
    );
  }

  display.display();
}