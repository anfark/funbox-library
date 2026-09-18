#pragma once

#include "core/Direction.h"

class Game {
public:
    explicit Game(const char* name);
    virtual ~Game() = default;

    const char* name() const;

    virtual void direction(Direction direction) = 0;
    virtual void click() = 0;
    virtual void tick() = 0;

    virtual bool isOver() const = 0;

private:
    const char* _name;
};