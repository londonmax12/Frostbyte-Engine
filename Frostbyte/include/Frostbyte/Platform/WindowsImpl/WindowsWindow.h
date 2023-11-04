#pragma once

#include "Platform/Interfaces/IWindow.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS

#include <Windows.h>

namespace Frostbyte {
	class WindowsWindow : IWindow
	{
	public:
		bool OnInit(const WindowConfig& config) override;
		void OnShutdown() override;
		void OnUpdate() override;
		
	private:
		struct WindowsWindowData
		{
			WindowsWindowData(HWND handle)
				: Handle(handle){}

			HWND Handle;
		};
	};
}
#endif