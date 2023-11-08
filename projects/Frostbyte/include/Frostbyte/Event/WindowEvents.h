#pragma once

#include "Event/IEvent.h"
#include "Math/Vector.h"

namespace Frostbyte {
    class WindowCloseEvent : public IEvent {
    public:
        EventType GetType() override { return EVENT_WINDOW_CLOSE; }
    };
    class WindowResizeEvent : public IEvent {
    public:
        WindowResizeEvent(VectorInt2 size = VectorInt2())
            : Size(size) {}

        EventType GetType() override { return EVENT_WINDOW_RESIZE; }

        VectorInt2 Size = {};
    };
}