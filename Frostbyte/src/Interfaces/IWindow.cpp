#include "Interfaces/IWindow.h"
#include "Platform/WindowsImpl/WindowsWindow.h"

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
#else
	return nullptr;
#endif

}
