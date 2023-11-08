#include "Platform/LinuxImpl/LinuxPlatform.h"

#ifdef FROSTBYTE_PLATFORM_LINUX

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

void Frostbyte::LinuxPlatform::GetVulkanExtentions(int* outCount, const char*** outNames)
{
	*outCount = 1;
	*outNames = new const char* [1];
	(*outNames)[0] = { VK_KHR_XCB_SURFACE_EXTENSION_NAME };
}
#endif