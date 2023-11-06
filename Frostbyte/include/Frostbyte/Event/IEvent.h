#pragma once

namespace Frostbyte {
    enum EventType {
        EVENT_WINDOW_CLOSE,
        EVENT_WINDOW_RESIZE,
    };

    class IEvent {
    public:
        virtual ~IEvent() = default;

        virtual EventType GetType() = 0;
    };
}