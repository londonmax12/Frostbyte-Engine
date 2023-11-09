#pragma once

#include <vulkan/vulkan.h>

namespace Frostbyte {
	class VulkanDevice;

	struct VulkanContext {
		VkInstance Instance;
		VulkanDevice* Device;

		bool ValidationLayersEnabled;
		uint32_t ValidationLayerCount;
		const char** ValidationLayers;
	};
}