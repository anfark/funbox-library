#include "Settings.h"

#include <Preferences.h>


namespace {

constexpr const char* PREFERENCES_NAMESPACE = "funbox";

constexpr const char* BRIGHTNESS_KEY = "brightness";
constexpr const char* VOLUME_KEY = "volume";

}


// ==================================================
// Construction
// ==================================================

Settings::Settings(
  Screen& screen,
  Stick& stick,
  Matrix& matrix,
  Audio& audio
)
  : _screen(screen),
    _stick(stick),
    _matrix(matrix),
    _audio(audio) {
}


// ==================================================
// Setup
// ==================================================

void Settings::setup() {
  load();

  applyBrightness();
  applyVolume();
}


void Settings::start() {
  _selectedOption = Option::Brightness;

  // Aktuellen Stick-Zustand übernehmen.
  // So löst eine noch gehaltene Taste beim Öffnen
  // des Menüs nicht sofort eine Aktion aus.
  _upWasPressed =
    _stick.isPressed(StickKey::UP);

  _downWasPressed =
    _stick.isPressed(StickKey::DOWN);

  _leftWasPressed =
    _stick.isPressed(StickKey::LEFT);

  _rightWasPressed =
    _stick.isPressed(StickKey::RIGHT);

  draw();
}


void Settings::update() {
  handleInput();
}


// ==================================================
// Persistence
// ==================================================

void Settings::load() {
  Preferences preferences;

  if (!preferences.begin(
        PREFERENCES_NAMESPACE,
        false
      )) {
    _brightness = DEFAULT_BRIGHTNESS;
    _volume = DEFAULT_VOLUME;
    return;
  }

  _brightness =
    preferences.getUChar(
      BRIGHTNESS_KEY,
      DEFAULT_BRIGHTNESS
    );

  _volume =
    preferences.getUChar(
      VOLUME_KEY,
      DEFAULT_VOLUME
    );

  if (_brightness > MAX_VALUE) {
    _brightness = DEFAULT_BRIGHTNESS;
  }

  if (_volume > MAX_VALUE) {
    _volume = DEFAULT_VOLUME;
  }

  if (!preferences.isKey(BRIGHTNESS_KEY)) {
    preferences.putUChar(
      BRIGHTNESS_KEY,
      _brightness
    );
  }

  if (!preferences.isKey(VOLUME_KEY)) {
    preferences.putUChar(
      VOLUME_KEY,
      _volume
    );
  }

  preferences.end();
}


void Settings::saveBrightness() {
  Preferences preferences;

  if (!preferences.begin(
        PREFERENCES_NAMESPACE,
        false
      )) {
    return;
  }

  preferences.putUChar(
    BRIGHTNESS_KEY,
    _brightness
  );

  preferences.end();
}


void Settings::saveVolume() {
  Preferences preferences;

  if (!preferences.begin(
        PREFERENCES_NAMESPACE,
        false
      )) {
    return;
  }

  preferences.putUChar(
    VOLUME_KEY,
    _volume
  );

  preferences.end();
}


// ==================================================
// Input
// ==================================================

void Settings::handleInput() {
  const bool upPressed =
    _stick.isPressed(StickKey::UP);

  const bool downPressed =
    _stick.isPressed(StickKey::DOWN);

  const bool leftPressed =
    _stick.isPressed(StickKey::LEFT);

  const bool rightPressed =
    _stick.isPressed(StickKey::RIGHT);


  if (upPressed && !_upWasPressed) {
    selectPrevious();
  }

  if (downPressed && !_downWasPressed) {
    selectNext();
  }

  if (leftPressed && !_leftWasPressed) {
    decreaseValue();
  }

  if (rightPressed && !_rightWasPressed) {
    increaseValue();
  }


  _upWasPressed = upPressed;
  _downWasPressed = downPressed;
  _leftWasPressed = leftPressed;
  _rightWasPressed = rightPressed;
}


// ==================================================
// Selection
// ==================================================

void Settings::selectPrevious() {
  if (_selectedOption == Option::Brightness) {
    _selectedOption = Option::Volume;
  } else {
    _selectedOption = Option::Brightness;
  }

  draw();
}


void Settings::selectNext() {
  if (_selectedOption == Option::Brightness) {
    _selectedOption = Option::Volume;
  } else {
    _selectedOption = Option::Brightness;
  }

  draw();
}


// ==================================================
// Values
// ==================================================

void Settings::decreaseValue() {
  switch (_selectedOption) {
    case Option::Brightness:
      if (_brightness > MIN_VALUE) {
        _brightness--;

        applyBrightness();
        saveBrightness();
        draw();
      }
      break;


    case Option::Volume:
      if (_volume > MIN_VALUE) {
        _volume--;

        applyVolume();
        saveVolume();
        draw();
      }
      break;


    case Option::Count:
      break;
  }
}


void Settings::increaseValue() {
  switch (_selectedOption) {
    case Option::Brightness:
      if (_brightness < MAX_VALUE) {
        _brightness++;

        applyBrightness();
        saveBrightness();
        draw();
      }
      break;


    case Option::Volume:
      if (_volume < MAX_VALUE) {
        _volume++;

        applyVolume();
        saveVolume();
        draw();
      }
      break;


    case Option::Count:
      break;
  }
}


// ==================================================
// Hardware
// ==================================================

void Settings::applyBrightness() {
  _matrix.brightness(
    _brightness
  );
}


void Settings::applyVolume() {
  _audio.volume(
    _volume
  );
}


// ==================================================
// Drawing
// ==================================================

void Settings::draw() {
  auto& display = _screen.display();

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SETTINGS");

  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE
  );


  // Brightness

  display.setCursor(0, 20);

  if (_selectedOption == Option::Brightness) {
    display.print("> ");
  } else {
    display.print("  ");
  }

  display.print("Brightness  ");
  display.println(_brightness);


  // Volume

  display.setCursor(0, 34);

  if (_selectedOption == Option::Volume) {
    display.print("> ");
  } else {
    display.print("  ");
  }

  display.print("Volume      ");
  display.println(_volume);


  // Help

  display.setCursor(0, 52);
  display.println("UP/DOWN  LEFT/RIGHT");

  display.display();
}