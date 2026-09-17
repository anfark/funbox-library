/*
#pragma once
#include <functional>
#include <optional>
#include <variant>
#include <vector>

#include "snake/Direction.h"
#include "snake/Position.h"
#include "snake/Size.h"


using GeneratePosition = std::function<Position(Size size)>;

struct GameConfig {
    Size bounds;
    GeneratePosition generatePosition;

    Position generatePositionInBounds() const {
        return generatePosition(bounds);
    }
};

using SnakeBody = std::vector<Position>;

struct Snake {
    Position head;
    SnakeBody rest;
};

struct GameState {
    Snake snake;
    Direction direction;
    Position food;
};

struct ChangeDirection {
    Direction newDirection;
};

struct Move {};

using GameAction = std::variant<
    Move, 
    ChangeDirection
>;

struct DidEatFruit {};

using GameEvent = std::variant<
    DidEatFruit
>;

using EventQueue = std::vector<GameEvent>;

class Game {
public:
    explicit Game(GameConfig config);
    
    void action(const GameAction& action);

    const GameState& state() const;
    std::optional<GameEvent> nextEvent();

private:
    const GameConfig _config;
    GameState _state;
    EventQueue _queue;

    void _event(GameEvent event);
};
*/