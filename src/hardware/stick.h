#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <vector>


enum class StickKey : uint8_t {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  MID,
  SET,
  RESET,
  COUNT
};


static constexpr uint8_t STICK_COUNT =
  static_cast<uint8_t>(StickKey::COUNT);


inline const char* toString(StickKey key) {
  switch (key) {
    case StickKey::UP:    return "UP";
    case StickKey::DOWN:  return "DOWN";
    case StickKey::LEFT:  return "LEFT";
    case StickKey::RIGHT: return "RIGHT";
    case StickKey::MID:   return "MID";
    case StickKey::SET:   return "SET";
    case StickKey::RESET: return "RESET";
    default:              return "UNKNOWN";
  }
}


struct StickEvent {
  StickKey key;
  bool isPressed;

  String toString() const {
    return String(::toString(key))
      + " "
      + (isPressed ? "pressed" : "released");
  }
};


struct StickPin {
  uint8_t up;
  uint8_t down;
  uint8_t left;
  uint8_t right;
  uint8_t mid;
  uint8_t set;
  uint8_t reset;
};


class Stick {
public:
  explicit Stick(StickPin pin);

  void setup();
  void update();

  bool isPressed(StickKey key) const;

  const std::vector<StickEvent>& events() const;

private:
  uint8_t _pins[STICK_COUNT];
  bool _states[STICK_COUNT] = {};

  std::vector<StickEvent> _events;

  void readInputs(bool states[STICK_COUNT]);
  void triggerEvents(const bool states[STICK_COUNT]);

  static uint8_t indexOf(StickKey key);
};