#pragma once

#include <functional>

template<typename Event>
using PubFun = std::function<void(const Event&)>;

template<typename Event>
using SubFun = std::function<void(const PubFun<Event>&)>;

template<typename Event>
class Fan {
public:
    void operator()(PubFun<Event> fun) {
        _listeners.push_back(std::move(fun));
    }

    void operator()(Event evt) {
        for(const auto listener : _listeners) {
            listener(evt);
        }
    }

private:
    std::vector<PubFun<Event>> _listeners;
};