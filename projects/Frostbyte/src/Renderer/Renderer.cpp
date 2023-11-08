#include "Renderer/Renderer.h"

bool Frostbyte::Renderer::Init(RendererBackendType type)
{
    switch (type)
    {
    case Renderer::RENDERER_BACKEND_VULKAN:
        m_Backend = new VulkanRendererBackend();
        break;
    default:
        return false;
    }

    if (!m_Backend->Init()) {
        FROSTBYTE_ERROR("Failed to initialize renderer backend");
        return false;
    };

    return true;
}

void Frostbyte::Renderer::Update()
{
    if (m_Backend)
        m_Backend->Update();
}

void Frostbyte::Renderer::Shutdown()
{
    if (m_Backend)
    {
        m_Backend->Shutdown();
        delete m_Backend;
    }
}