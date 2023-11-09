#pragma once

#include <vulkan/vulkan.h>

namespace Frostbyte {
	class VulkanDevice;

	struct VulkanContext {
		VkInstance Instance;
		VulkanDevice* Device;
	};
}