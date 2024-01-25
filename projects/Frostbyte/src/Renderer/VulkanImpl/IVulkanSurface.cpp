#include "Renderer/VulkanImpl/IVulkanSurface.h"

#include "Platform/PlatformDetection.h"

#include "Platform/WindowsImpl/Vulkan/WindowsVulkanSurface.h"

Frostbyte::IVulkanSurface* Frostbyte::IVulkanSurface::Create()
{
#ifdef FROSTBYTE_PLATFORM_WINDOWS
	return (IVulkanSurface*)(new WindowsVulkanSurface());
#elif defined(FROSTBYTE_PLATFORM_LINUX)
	return nullptr;
#else
	return nullptr;
#endif
}