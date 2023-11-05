#pragma once

namespace Frostbyte {
    enum EventType {
        EVENT_WINDOW_CLOSE,
    };

    class IEvent {
    public:
        virtual ~IEvent() = default;

        virtual EventType GetType() = 0;
    };
}