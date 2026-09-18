#pragma once

#include <utility>
#include <vector>

#include "BaseGame.h"
#include "GameDevices.h"
#include "GameRender.h"


template<typename Definition>
class Game
  : public BaseGame {

public:
  using State =
    typename Definition::State;

  using Event =
    typename Definition::Event;

  using Update =
    typename Definition::Update;

  using Render =
    typename Definition::Render;

  using Events =
    std::vector<Event>;


  explicit Game(
    GameDevices devices
  )
    : _state(
        Definition::initialState()
      ),

      _update(
        _state,
        [this](Event event) {
          _events.push_back(
            std::move(event)
          );
        }
      ),

      _render(
        devices
      ) {
  }


  // Erzeugt einen neuen Startzustand des Spiels.
  void reset() override {
    _state =
      Definition::initialState();

    _events.clear();
  }


  // Zeichnet den aktuellen Zustand des Spiels.
  void render() override {
    renderGame(
      _render,
      _state,
      _events
    );

    _events.clear();
  }


  // Zeichnet den finalen Zustand des Spiels.
  void renderGameOver() override {
    ::renderGameOver(
      _render,
      _state
    );

    _events.clear();
  }


  // Führt einen Zeitschritt des Spiels aus.
  void tick() override {
    dispatch(
      [this] {
        _update.tick();
      }
    );
  }


  // Leitet eine Bewegungsrichtung an das Spiel weiter.
  void move(
    Direction direction
  ) override {
    dispatch(
      [this, direction] {
        _update.move(
          direction
        );
      }
    );
  }


  // Leitet einen Klick an das Spiel weiter.
  void click() override {
    dispatch(
      [this] {
        _update.click();
      }
    );
  }


  // Prüft, ob das Spiel beendet ist.
  bool isOver() const override {
    return _update.isOver();
  }


private:
  // Führt eine Action aus und rendert das Ergebnis.
  template<typename Action>
  void dispatch(
    Action action
  ) {
    _events.clear();

    action();

    renderGame(
      _render,
      _state,
      _events
    );

    _events.clear();
  }


  State _state;

  Events _events;

  Update _update;
  Render _render;
};