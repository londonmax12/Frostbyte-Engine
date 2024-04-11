#include "Platform/PlatformDetection.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR 
#include "Platform/WindowsImpl/Vulkan/WindowsVulkanSurface.h"

#include "Application/Application.h"
#include "Platform/WindowsImpl/WindowsWindow.h"

bool Frostbyte::WindowsVulkanSurface::Init(const VulkanContext& context)
{
	m_Context = context;
	WindowsWindow::WindowsWindowData* windowData = (WindowsWindow::WindowsWindowData*)Application::GetInstance()->GetWindow()->RawData();

	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hwnd = windowData->Handle;
	surfaceCreateInfo.hinstance = GetModuleHandleA(NULL);

	if (vkCreateWin32SurfaceKHR(context.Instance, &surfaceCreateInfo, nullptr, &m_Surface) != VK_SUCCESS) {
		FROSTBYTE_FATAL("Failed to create Win32 Vulkan surface");
		return false;
	}

	return true;
}

void Frostbyte::WindowsVulkanSurface::Shutdown()
{
	vkDestroySurfaceKHR(m_Context.Instance, m_Surface, nullptr);
}
#endif

