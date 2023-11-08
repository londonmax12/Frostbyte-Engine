#pragma once

#include "Frostbyte/Renderer/VulkanImpl/VulkanRendererBackend.h"
#include "Frostbyte/Logging/Logging.h"

namespace Frostbyte {
    class Renderer {
    public:
        enum RendererBackendType {
            RENDERER_BACKEND_VULKAN
        };

        bool Init(RendererBackendType type);
        void Update();
        void Shutdown();

    private:
        IRendererBackend* m_Backend = nullptr;
    };
}