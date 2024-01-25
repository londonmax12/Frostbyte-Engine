#pragma once

#include "vulkan/vulkan.h"
#include "VulkanStructs.h"

namespace Frostbyte {
	class IVulkanSurface
	{
	public:
		static IVulkanSurface* Create();

		virtual bool Init(const VulkanContext& context) = 0;
		virtual void Shutdown() = 0;

	protected:
		VkSurfaceKHR m_Surface;
	};
}