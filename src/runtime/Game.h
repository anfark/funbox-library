#pragma once

#include <functional>
#include <utility>
#include <vector>

#include "core/Direction.h"
#include "GameRender.h"


class RunningGame {
public:
  virtual ~RunningGame() = default;

  virtual void reset() = 0;
  virtual void render() = 0;

  virtual void tick() = 0;
  virtual void click() = 0;
  virtual void move(Direction direction) = 0;

  virtual bool isOver() const = 0;
};


template<
  typename State,
  typename Event,
  typename Update,
  typename Render
>
class Game : public RunningGame {
public:
  using Events =
    std::vector<Event>;

  using Emit =
    std::function<void(Event)>;


protected:
  explicit Game(
    State initialState
  )
    : _initialState(initialState),
      _state(std::move(initialState)) {
  }


  State& state() {
    return _state;
  }


  const State& state() const {
    return _state;
  }


  Emit emit() {
    return [this](Event event) {
      _events.push_back(
        std::move(event)
      );
    };
  }


  void connect(
    Update& update,
    Render& render
  ) {
    _update = &update;
    _render = &render;
  }


public:
  void reset() override {
    _state = _initialState;
    _events.clear();
  }


  void render() override {
    if (_render == nullptr) {
      return;
    }

    renderGame(
      *_render,
      _state,
      _events
    );

    _events.clear();
  }


  void tick() override {
    dispatch(
      [this] {
        _update->tick();
      }
    );
  }


  void click() override {
    dispatch(
      [this] {
        _update->click();
      }
    );
  }


  void move(
    Direction direction
  ) override {
    dispatch(
      [this, direction] {
        _update->move(
          direction
        );
      }
    );
  }


  bool isOver() const override {
    return
      _update != nullptr &&
      _update->isOver();
  }


private:
  template<typename Action>
  void dispatch(
    Action action
  ) {
    if (
      _update == nullptr ||
      _render == nullptr
    ) {
      return;
    }

    _events.clear();

    action();

    renderGame(
      *_render,
      _state,
      _events
    );

    _events.clear();
  }


  State _initialState;
  State _state;

  Events _events;

  Update* _update = nullptr;
  Render* _render = nullptr;
};