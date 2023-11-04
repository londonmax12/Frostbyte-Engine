#include "Application/Application.h"

#include "Logging/Logging.h"

bool Frostbyte::Application::Init()
{
    if (!m_SceneGraph.Init()) {
        FROSTBYTE_FATAL("Failed to initialize scene graph");
        return false;
    }

    if (!OnInit()) {
        FROSTBYTE_FATAL("Client returned false");
        return false;
    }

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
}

void Frostbyte::Application::Shutdown()
{
    m_SceneGraph.Shutdown();
}
