#include "Application/Application.h"
#include "Application/EventDispatcher.h"

#include "Logging/Logging.h"

bool Frostbyte::Application::Init()
{
    if (m_Instance) {
        FROSTBYTE_FATAL("Failed to initialize application: Instance already exists");
        return false;
    }
    m_Instance = this;

    FROSTBYTE_INFO("Initializing scene graph");
    if (!m_SceneGraph.Init()) {
        FROSTBYTE_FATAL("Failed to initialize scene graph");
        return false;
    }

    FROSTBYTE_INFO("Initializing event dispatcher");
    if (!EventDispatcher::Init()) {
        FROSTBYTE_ERROR("Failed to initialize event dispatcher");
        return false;
    }
    
    FROSTBYTE_INFO("Setting application event listeners");

    EventDispatcher::GetInstance()->AddListener(EVENT_WINDOW_CLOSE, [&](IEvent* ev) -> void { OnEvent(ev); });

    if (!OnInit()) {
        FROSTBYTE_FATAL("Client returned false");
        return false;
    }
        
    FROSTBYTE_INFO("Creating window");
    m_Window = (IWindow*)IWindow::Create();
    if (!m_Window) {
        FROSTBYTE_ERROR("Failed to create window");
        return false;
    }

    if (!m_Window->Init(Config.WindowConf)) {
        FROSTBYTE_ERROR("Failed to initialize window");
        return false;
    }

    return true;
}

void Frostbyte::Application::Update()
{
    m_SceneGraph.Render();
    m_Window->Update();
    EventDispatcher::GetInstance()->PollEvents();
}

void Frostbyte::Application::Shutdown()
{
    m_SceneGraph.Shutdown();
    m_Window->Shutdown();
    EventDispatcher::Shutdown();
}

void Frostbyte::Application::OnEvent(IEvent* ev)
{
    m_Running = false;
}
