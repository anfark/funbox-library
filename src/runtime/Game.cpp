#include "Game.h"

Game::Game(const char* name)
    : _name(name) {
}

const char* Game::name() const {
    return _name;
}