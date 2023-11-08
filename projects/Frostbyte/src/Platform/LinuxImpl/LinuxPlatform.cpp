#include "Platform/LinuxImpl/LinuxPlatform.h"

#ifdef FROSTBYTE_PLATFORM_LINUX

#include <xcb/xcb.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

#include <iostream>

void Frostbyte::LinuxPlatform::GetVulkanExtentions(int* outCount, const char*** outNames)
{
	*outCount = 1;
	*outNames = new const char* [1];
	(*outNames)[0] = { VK_KHR_XCB_SURFACE_EXTENSION_NAME };
}
void Frostbyte::LinuxPlatform::Print(std::string msg)
{
	std::cout << msg << "\n";
}
void Frostbyte::LinuxPlatform::PrintWarning(std::string msg)
{
	std::cout << msg << "\n";
}
void Frostbyte::LinuxPlatform::PrintError(std::string msg)
{
	std::cerr << msg << "\n";
}
#endif