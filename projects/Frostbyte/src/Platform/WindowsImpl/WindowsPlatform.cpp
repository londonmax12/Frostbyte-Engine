#include "Platform/WindowsImpl/WindowsPlatform.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS

#include <Windows.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

void Frostbyte::WindowsPlatform::GetVulkanExtentions(int* outCount, const char*** outNames)
{
	*outCount = 1;
	*outNames = new const char* [1];
	(*outNames)[0] = { VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
}
#endif