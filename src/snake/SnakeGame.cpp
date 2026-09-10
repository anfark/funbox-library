#include "snake/SnakeGame.h"

SnakeGame::SnakeGame()
    : _head{0, 0},
      _direction(Direction::Right) {}

void SnakeGame::update() {
    switch (_direction) {
        case Direction::Up:
            _head.y--;
            break;

        case Direction::Down:
            _head.y++;
            break;

        case Direction::Left:
            _head.x--;
            break;

        case Direction::Right:
            _head.x++;
            break;
    }
}

void SnakeGame::setDirection(Direction direction) {
    if (_direction == inverseDirection(direction)) {
        return;
    }

    _direction = direction;
}

Position SnakeGame::head() const {
    return _head;
}