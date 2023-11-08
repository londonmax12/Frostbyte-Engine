#include "Application/IWindow.h"
#include "Platform/WindowsImpl/WindowsWindow.h"
#include "Platform/LinuxImpl/LinuxWindow.h"

bool Frostbyte::IWindow::Init(const WindowConfig& config)
{
	return OnInit(config);
}

void Frostbyte::IWindow::Shutdown()
{
	OnShutdown();

	if (m_WindowData)
	{
		delete m_WindowData;
		m_WindowData = nullptr;
	}
}

void Frostbyte::IWindow::Update()
{
	OnUpdate();
}

Frostbyte::IWindow* Frostbyte::IWindow::Create()
{
#ifdef FROSTBYTE_PLATFORM_WINDOWS
	return (IWindow*)(new WindowsWindow());
#elif defined(FROSTBYTE_PLATFORM_LINUX)
	return (IWindow*)(new LinuxWindow());
#else
	return nullptr;
#endif
}
