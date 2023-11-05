#include "Application/EventDispatcher.h"

void Frostbyte::EventDispatcher::AddListener(EventType type, std::function<void(IEvent*)> callback)
{
    m_Listeners[type].emplace_back([callback](IEvent* event) {
        callback(event);
    });
}

void Frostbyte::EventDispatcher::PollEvents()
{
    while (!m_EventQueue.empty()) {
        IEvent* ev = m_EventQueue.front();
        auto listeners = m_Listeners[ev->GetType()];
        for (auto& listener : listeners) {
            listener(ev);
        }
        m_EventQueue.pop();
        delete ev;
    }
}

void Frostbyte::EventDispatcher::QueueEvent(IEvent* ev)
{
    m_EventQueue.push(ev);
}

bool Frostbyte::EventDispatcher::Init()
{
    if (m_Instance)
        return false;

    m_Instance = new EventDispatcher();
    m_Instance->m_Listeners = std::unordered_map<EventType, std::vector<std::function<void(IEvent*)>>>();
    return true;
}

void Frostbyte::EventDispatcher::Shutdown()
{
    if (m_Instance) {
        delete m_Instance;
        m_Instance = nullptr;
    }
}
