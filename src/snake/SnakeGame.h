#pragma once

#include "snake/Direction.h"

struct Position {
    int x;
    int y;
};

class SnakeGame {
public:
    SnakeGame();

    void update();
    void setDirection(Direction direction);

    Position head() const;

private:
    Position _head;
    Direction _direction;
};