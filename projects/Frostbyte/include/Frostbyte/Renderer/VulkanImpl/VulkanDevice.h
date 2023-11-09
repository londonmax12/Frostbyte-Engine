#pragma once

#include "Renderer/VulkanImpl/VulkanStructs.h"

namespace Frostbyte {
	class VulkanDevice {
	public:
		bool Init(VulkanContext* context);
		void Shutdown();

	private:
		bool PickDevice(VulkanContext* context);

		VkPhysicalDevice m_Physical = VK_NULL_HANDLE;
	};
}