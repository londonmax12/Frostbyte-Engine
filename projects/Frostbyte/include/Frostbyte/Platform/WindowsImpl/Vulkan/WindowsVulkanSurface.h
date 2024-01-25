#pragma once

#include "Platform/PlatformDetection.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS

#include "Renderer/VulkanImpl/IVulkanSurface.h"

namespace Frostbyte {
	class WindowsVulkanSurface : public IVulkanSurface
	{
		public:
			bool Init() override;
			void Shutdown() override;
	};
}
#endif