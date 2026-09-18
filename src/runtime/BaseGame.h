#pragma once

#include "core/Direction.h"
#include "core/Size.h"
#include "hardware/Matrix.h"


class BaseGame {
public:
  static constexpr Size bounds =
    Matrix::SIZE;

  virtual ~BaseGame() = default;

  virtual void reset() = 0;

  virtual void render() = 0;
  virtual void renderGameOver() = 0;

  virtual void tick() = 0;
  virtual void move(Direction direction) = 0;
  virtual void click() = 0;

  virtual bool isOver() const = 0;
};