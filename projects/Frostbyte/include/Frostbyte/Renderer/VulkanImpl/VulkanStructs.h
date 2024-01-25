#pragma once

#include <vulkan/vulkan.h>

namespace Frostbyte {
	class VulkanDevice;
	class IVulkanSurface;

	struct VulkanContext {
		VkInstance Instance;
		VulkanDevice* Device;
		IVulkanSurface* Surface;

		bool ValidationLayersEnabled;
		uint32_t ValidationLayerCount;
		const char** ValidationLayers;
	};
}