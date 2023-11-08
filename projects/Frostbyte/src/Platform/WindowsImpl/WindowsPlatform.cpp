#include "Platform/WindowsImpl/WindowsPlatform.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS

#include <Windows.h>
#include <iostream>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
void Frostbyte::WindowsPlatform::GetVulkanExtentions(int* outCount, const char*** outNames)
{
	*outCount = 1;
	*outNames = new const char* [1];
	(*outNames)[0] = { VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
}
void Frostbyte::WindowsPlatform::Print(std::string msg)
{
	std::cout << msg << "\n";
}
void Frostbyte::WindowsPlatform::PrintWarning(std::string msg)
{
	std::cout << "\x1B[33m" << msg << "\033[0m" << "\n";
}
void Frostbyte::WindowsPlatform::PrintError(std::string msg)
{
	std::cerr << "\x1B[31m" << msg << "\033[0m" << "\n";
}
#endif