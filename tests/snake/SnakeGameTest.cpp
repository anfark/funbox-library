#include <gtest/gtest.h>

#include "SnakeGame.h"

TEST(SnakeGameTest, MovesOneCellToTheRightOnUpdate) {
    SnakeGame game;

    const auto initialHead = game.head();

    game.update();

    EXPECT_EQ(game.head().x, initialHead.x + 1);
    EXPECT_EQ(game.head().y, initialHead.y);
}

TEST(SnakeGameTest, MovesUpAfterChangingDirection) {
    SnakeGame game;

    game.setDirection(Direction::Up);

    const auto initialHead = game.head();

    game.update();

    EXPECT_EQ(game.head().x, initialHead.x);
    EXPECT_EQ(game.head().y, initialHead.y - 1);
}

TEST(SnakeGameTest, CannotReverseDirection) {
    SnakeGame game;

    game.setDirection(Direction::Left);
    game.update();

    EXPECT_EQ(game.head().x, 1);
}