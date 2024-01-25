#include "Application/Application.h"
#include "Application/EventDispatcher.h"

#include "Logging/Logging.h"

#include "Platform/IPlatform.h"

bool Frostbyte::Application::Init()
{
    if (!IPlatform::Init()) {
        FROSTBYTE_ERROR("Failed to initialize platform");
        return false;
    }
    FROSTBYTE_INFO("Initialized platform");

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

    FROSTBYTE_INFO("Initializing renderer");
    m_Renderer = new Renderer();
    if (!m_Renderer->Init(Renderer::RENDERER_BACKEND_VULKAN)) {
        FROSTBYTE_ERROR("Failed to initialize renderer");
        return false;
    }

    return true;
}

void Frostbyte::Application::Update()
{
    EventDispatcher* eventDispatcher = EventDispatcher::GetInstance();

    m_SceneGraph.Render();

    if (m_Window)
        m_Window->Update();

    if (m_Renderer)
        m_Renderer->Update();

    if (eventDispatcher)
        eventDispatcher->PollEvents();
}

void Frostbyte::Application::Shutdown()
{
    m_SceneGraph.Shutdown();

    if (m_Window)
    {
        FROSTBYTE_INFO("Destroying window");
        m_Window->Shutdown();
        delete m_Window;
    }

    FROSTBYTE_INFO("Destroying event dispatcher");
    EventDispatcher::Shutdown();

    if (m_Renderer) {
        FROSTBYTE_INFO("Destroying renderer");
        m_Renderer->Shutdown();
        delete m_Renderer;
    }

    FROSTBYTE_INFO("Destroying platform interface");
    IPlatform::Shutdown();
}

void Frostbyte::Application::OnEvent(IEvent* ev)
{
    m_Running = false;
}
