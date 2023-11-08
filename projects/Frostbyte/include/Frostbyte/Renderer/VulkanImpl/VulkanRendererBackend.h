#pragma once

#include "Renderer/IRendererBackend.h"

#include <vector>

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
		bool HasValidationLayers(const std::vector<const char*> validationLayers);

		VkInstance m_Instance;

#ifdef _DEBUG
		bool CreateMessenger();

		VkDebugUtilsMessengerEXT m_DebugMessenger;
#endif

	};
}