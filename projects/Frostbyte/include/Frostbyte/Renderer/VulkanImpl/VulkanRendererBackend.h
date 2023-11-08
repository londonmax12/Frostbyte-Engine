#pragma once

#include "Renderer/IRendererBackend.h"

#include <vulkan/vulkan.h>

namespace Frostbyte {
	class VulkanRendererBackend : public IRendererBackend
	{
	public:
		bool Init() override;
		void Update() override;
		void Shutdown() override;

	private:
		bool CreateInstance();

		VkInstance m_Instance;
	};
}