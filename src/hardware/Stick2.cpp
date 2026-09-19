#include "Stick.h"

#include <Arduino.h>


Stick::Stick(StickPin pin)
  : _pins{
      pin.up,
      pin.down,
      pin.left,
      pin.right,
      pin.mid,
      pin.set,
      pin.reset
    } {
}


void Stick::setup() {
  for (uint8_t i = 0; i < STICK_COUNT; i++) {
    pinMode(_pins[i], INPUT_PULLUP);
    _states[i] = false;
  }

  // Initialzustand lesen, damit beim Start
  // keine falschen Events erzeugt werden.
  readInputs(_states);
}


void Stick::update() {
  _events.clear();

  bool newStates[STICK_COUNT] = {};

  readInputs(newStates);
  triggerEvents(newStates);
}


bool Stick::isPressed(StickKey key) const {
  return _states[indexOf(key)];
}


const std::vector<StickEvent>& Stick::events() const {
  return _events;
}


void Stick::readInputs(
  bool states[STICK_COUNT]
) {
  for (uint8_t i = 0; i < STICK_COUNT; i++) {
    // INPUT_PULLUP:
    // gedrückt = LOW
    states[i] =
      digitalRead(_pins[i]) == LOW;
  }
}


void Stick::triggerEvents(
  const bool states[STICK_COUNT]
) {
  for (uint8_t i = 0; i < STICK_COUNT; i++) {
    if (states[i] == _states[i]) {
      continue;
    }

    _states[i] = states[i];

    _events.push_back({
      .key = static_cast<StickKey>(i),
      .isPressed = _states[i],
    });
  }
}


uint8_t Stick::indexOf(StickKey key) {
  return static_cast<uint8_t>(key);
}