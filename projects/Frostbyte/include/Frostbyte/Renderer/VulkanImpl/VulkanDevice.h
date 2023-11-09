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
		bool CreateLogicalDevice(VulkanContext* context);

		QueueFamilyIndices m_QueueIndices;

		VkQueue m_GraphicsQueue;
		VkPhysicalDevice m_Physical = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;
	};
}