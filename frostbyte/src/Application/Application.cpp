#include "Application/Application.h"

bool Frostbyte::Application::Init()
{
    if (!m_SceneGraph.Init())
        return false;
    return OnInit();
}

void Frostbyte::Application::Update()
{
    m_SceneGraph.Render();
}

void Frostbyte::Application::Shutdown()
{
    m_SceneGraph.Shutdown();
}
