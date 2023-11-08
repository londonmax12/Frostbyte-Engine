#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <queue>

#include "Event/IEvent.h"

namespace Frostbyte {
    class EventDispatcher {
    public:
        EventDispatcher() = default;

        void AddListener(EventType type, std::function<void(IEvent*)> callback);
        void PollEvents();
        void QueueEvent(IEvent* ev);

        static bool Init();
        static void Shutdown();
        static EventDispatcher* GetInstance() { return m_Instance; }

    private:
        inline static EventDispatcher* m_Instance = nullptr;
        std::unordered_map<EventType, std::vector<std::function<void(IEvent*)>>> m_Listeners;
        std::queue<IEvent*> m_EventQueue;
    };

}