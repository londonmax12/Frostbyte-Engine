#pragma once

#include "Application/IWindow.h"

#ifdef FROSTBYTE_PLATFORM_LINUX

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

namespace Frostbyte {
	class LinuxWindow : IWindow
	{
	public:
		bool OnInit(const WindowConfig& config) override;
		void OnShutdown() override;
		void OnUpdate() override;
		void OnResize(VectorInt2) override;

	private:
		struct LinuxWindowData
		{
			LinuxWindowData();

			Display* _Display;
			xcb_connection_t* Connection;
        	xcb_window_t Window;
			xcb_screen_t* Screen;
			xcb_atom_t WMProtocols;
			xcb_atom_t WMDeleteWin;
		};
	};
}
#endif