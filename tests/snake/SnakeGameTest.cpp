#include <gtest/gtest.h>

#include "snake/SnakeGame.h"

TEST(SnakeGameTest, MovesOneCellToTheRightOnUpdate) {
    SnakeGame game;

    const auto initialHead = game.head();

    game.update();

    EXPECT_EQ(game.head(), initialHead + Direction::Right);
}

TEST(SnakeGameTest, MovesUpAfterChangingDirection) {
    SnakeGame game;

    game.setDirection(Direction::Up);

    const auto initialHead = game.head();

    game.update();

    EXPECT_EQ(game.head(), initialHead + Direction::Up);
}

TEST(SnakeGameTest, CannotReverseDirection) {
    SnakeGame game;

    const auto initialHead = game.head();

    game.setDirection(Direction::Left);
    game.update();

    EXPECT_EQ(game.head(), initialHead + Direction::Right);
}

TEST(SnakeGameTest, MovesBodyOneCellToTheRightOnUpdate) {
    SnakeGame game;

    game.update();

    EXPECT_EQ(
        game.body(),
        (SnakeBody{
            {3, 0},
            {2, 0},
            {1, 0}
        })
    );
}