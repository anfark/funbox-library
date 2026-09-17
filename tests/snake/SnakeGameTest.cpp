#include <gtest/gtest.h>
/*
#include "snake/SnakeGame.h"
#include "snake/Direction.h"
#include "snake/Position.h"
#include "snake/Size.h"

namespace {

GameConfig config() {
    return {
        .bounds = {
            .w = 8,
            .h = 8,
        },
        .generatePosition = [](Size size) {
            return Position::Zero;
        }
    };
}

}

TEST(GameTest, MovesOneCellToTheRight) {
    Game game(config());

    const auto initialHead = game.state().snake.head;

    game.action(Move{});

    EXPECT_EQ(
        game.state().snake.head,
        initialHead + Direction::Right
    );
}

TEST(GameTest, MovesUpAfterChangingDirection) {
    Game game(config());

    game.action(ChangeDirection{Direction::Up});

    const auto initialHead = game.state().snake.head;

    game.action(Move{});

    EXPECT_EQ(
        game.state().snake.head,
        initialHead + Direction::Up
    );
}

TEST(GameTest, CannotReverseDirection) {
    Game game(config());

    const auto initialDirection = game.state().direction;

    game.action(ChangeDirection{
        .newDirection = inverseDirection(initialDirection)
    });

    EXPECT_EQ(
        game.state().direction,
        initialDirection
    );
}

TEST(GameTest, MovesBodyOneCellToTheRight) {
    Game game(config());

    const auto initialDirection = game.state().direction;
    const auto initialHead = game.state().snake.head;

    game.action(Move{});

    EXPECT_EQ(
        game.state().snake.head,
        initialHead + initialDirection
    );

    EXPECT_EQ(
        game.state().snake.rest[0],
        initialHead
    );
}
    */