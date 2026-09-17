/*
#include <utility>
#include "snake/SnakeGame.h"
#include "snake/Direction.h"
#include "snake/Position.h"

GameState initialState(const GameConfig& config) {
    const Position head = {
        .x = config.bounds.w / 2,
        .y = config.bounds.h / 2
    };

    const auto seg0 = head + Direction::Up;
    const auto seg1 = seg0 + Direction::Left;
    const auto seg2 = seg1 + Direction::Down;

    return {
        .snake = {
            .head = head,
            .rest = { seg0, seg1, seg2 }
        },
        .direction = Direction::Down,
        .food = config.generatePositionInBounds()
    };
}

Game::Game(GameConfig config)
    : _config(std::move(config)),
      _state(initialState(config)) {}



void move(Snake& snake, Direction direction) {
    snake.rest.insert(
        snake.rest.begin(), 
        snake.head
    );

    snake.rest.pop_back();
    snake.head += direction;
}

void changeDirection(Direction& currentDirection, Direction newDirection) {
    if (currentDirection == inverseDirection(newDirection)) {
            return;
    }

    currentDirection = newDirection;
}

void Game::handle(const Move&) {
    move(_state.snake, _state.direction);
}

void Game::handle(const ChangeDirection& action) {
    if (_state.direction ==
        inverseDirection(action.newDirection)) {
        return;
    }

    _state.direction = action.newDirection;
}


void Game::action(const GameAction& action) {
    if (std::holds_alternative<Move>(action)) {
        move(_state.snake, _state.direction);
        return;
    }

    if (const auto* changeDirection =
            std::get_if<ChangeDirection>(&action)) {

        changeDirection(
            _state.direction,
            changeDirection->newDirection
        );

        return;
    }
}



const GameState& Game::state() const {
    return _state;
}

std::optional<GameEvent> Game::nextEvent() {
    if (_queue.empty()) {
        return std::nullopt;
    }

    auto event = std::move(_queue.front());
    _queue.erase(_queue.begin());

    return event;
}

void Game::_event(GameEvent event) {
    _queue.push_back(std::move(event));
}
    */