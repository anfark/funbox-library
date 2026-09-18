#pragma once

#include <variant>
#include <vector>

#include "hardware/Audio.h"
#include "hardware/Matrix.h"
#include "hardware/Screen.h"


class GameRender {
public:
  GameRender(
    Matrix& matrix,
    Screen& screen,
    Audio& audio
  )
    : matrix(matrix),
      screen(screen),
      audio(audio) {
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

  for (const auto& event : events) {
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