#pragma once

#include <functional>
#include <utility>

#include "core/Direction.h"


template<typename State, typename Event>
class GameUpdate {
public:
    using Emit = std::function<void(Event)>;

    GameUpdate(
        State& state,
        Emit emit
    )
        : state(state),
          event(std::move(emit)) {
    }

    virtual ~GameUpdate() = default;

    virtual void tick() {
    }

    virtual void click() {
    }

    virtual void move(Direction) {
    }

    virtual bool isOver() const {
        return false;
    }

protected:
    State& state;
    Emit event;
};