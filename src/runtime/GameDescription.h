#pragma once

#include "Game.h"


class GameDescription {
public:
    GameDescription(
        const char* name,
        RunningGame& game
    );

    const char* name() const;

    RunningGame& game() const;

private:
    const char* _name;
    RunningGame& _game;
};