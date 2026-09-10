#include "snake/SnakeGame.h"

SnakeGame::SnakeGame()
    : _body{
        {2, 0},
        {1, 0},
        {0, 0}
      },
      _direction(Direction::Right) {}

Position SnakeGame::head() const {
    return _body.front();
}


void SnakeGame::update() {
    const auto newHead = head() + _direction;

    _body.insert(_body.begin(), newHead);
    _body.pop_back();
}

void SnakeGame::setDirection(Direction direction) {
    if (_direction == inverseDirection(direction)) {
        return;
    }

    _direction = direction;
}

const SnakeBody& SnakeGame::body() const {
    return _body;
}