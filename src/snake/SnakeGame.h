#pragma once
#include <vector>

#include "snake/Direction.h"
#include "snake/Position.h"



using SnakeBody = std::vector<Position>;

class SnakeGame {
public:
    SnakeGame();

    void update();
    void setDirection(Direction direction);

    Position head() const;
    const SnakeBody& body() const;

private:
    SnakeBody _body;
    Direction _direction;
};