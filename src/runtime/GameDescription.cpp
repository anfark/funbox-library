#include "GameDescription.h"


GameDescription::GameDescription(
    const char* name,
    RunningGame& game
)
    : _name(name),
      _game(game) {
}


const char* GameDescription::name() const {
    return _name;
}


RunningGame& GameDescription::game() const {
    return _game;
}