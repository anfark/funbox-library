#pragma once

#include <variant>
#include <vector>

#include "BaseGame.h"
#include "GameDevices.h"


class GameRender {
public:
  static constexpr Size bounds =
    BaseGame::bounds;


  explicit GameRender(
    GameDevices devices
  )
    : matrix(devices.matrix),
      screen(devices.screen),
      audio(devices.audio) {
  }


protected:
  Matrix& matrix;
  Screen& screen;
  Audio& audio;
};


template<
  typename Renderer,
  typename State,
  typename Event
>
void renderGame(
  Renderer& renderer,
  const State& state,
  const std::vector<Event>& events
) {
  if constexpr (requires {
    renderer.state(state);
  }) {
    renderer.state(state);
  }

  for (
    const auto& event :
    events
  ) {
    std::visit(
      [&renderer](const auto& value) {
        if constexpr (requires {
          renderer.event(value);
        }) {
          renderer.event(value);
        }
      },
      event
    );
  }
}


template<
  typename Renderer,
  typename State
>
void renderGameOver(
  Renderer& renderer,
  const State& state
) {
  if constexpr (requires {
    renderer.gameOver(state);
  }) {
    renderer.gameOver(state);
  }
}