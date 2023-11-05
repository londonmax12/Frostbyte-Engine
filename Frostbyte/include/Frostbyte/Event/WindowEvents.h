#include "Event/IEvent.h"

namespace Frostbyte {
    class WindowCloseEvent : public IEvent {
        EventType GetType() override { return EVENT_WINDOW_CLOSE; }
    };
}