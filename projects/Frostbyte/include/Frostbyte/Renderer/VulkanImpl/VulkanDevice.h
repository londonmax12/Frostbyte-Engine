#pragma once

#include "Renderer/VulkanImpl/VulkanStructs.h"

namespace Frostbyte {
	class VulkanDevice {
	public:
		struct QueueFamilyIndices {
			uint32_t GraphicsQueue;
		};

		bool Init(VulkanContext* context);
		void Shutdown();

	private:
		bool PickDevice(VulkanContext* context);
		bool FindQueueFamilies(VkPhysicalDevice device);

		QueueFamilyIndices m_QueueIndices;

		VkPhysicalDevice m_Physical = VK_NULL_HANDLE;
	};
}