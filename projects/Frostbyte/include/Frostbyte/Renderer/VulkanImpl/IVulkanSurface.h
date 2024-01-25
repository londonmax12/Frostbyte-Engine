#pragma once

#include "vulkan/vulkan.h"

namespace Frostbyte {
	class IVulkanSurface
	{
	public:
		static IVulkanSurface* Create();

		virtual bool Init() = 0;
		virtual void Shutdown() = 0;

	private:
		VkSurfaceKHR surface;
	};
}