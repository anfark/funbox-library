#pragma once

#include <variant>
#include <vector>


template<
    typename Derived,
    typename State,
    typename Event
>
class GameRender {
public:
    using Events = std::vector<Event>;

    void render(
        const State& state,
        const Events& events
    ) {
        auto& self =
            static_cast<Derived&>(*this);

        if constexpr (requires {
            self.didChange(state);
        }) {
            self.didChange(state);
        }

        for (const auto& event : events) {
            std::visit(
                [&self](const auto& value) {
                    if constexpr (requires {
                        self.didTrigger(value);
                    }) {
                        self.didTrigger(value);
                    }
                },
                event
            );
        }
    }
};