#pragma once

#include <functional>
#include <utility>

#include "BaseGame.h"


template<
  typename State,
  typename Event
>
class GameUpdate {
public:
  using Emit =
    std::function<void(Event)>;

  static constexpr Size bounds =
    BaseGame::bounds;


  GameUpdate(
    State& state,
    Emit event
  )
    : state(state),
      event(std::move(event)) {
  }


  virtual ~GameUpdate() = default;


  virtual void tick() {
  }


  virtual void move(
    Direction
  ) {
  }


  virtual void click() {
  }


  virtual bool isOver() const {
    return false;
  }


protected:
  State& state;
  Emit event;
};