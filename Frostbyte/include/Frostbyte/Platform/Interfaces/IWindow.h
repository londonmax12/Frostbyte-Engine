#pragma once

#include "Platform/PlatformDetection.h"

namespace Frostbyte {
	struct WindowConfig {
		WindowConfig() = default;
		WindowConfig(int width, int height, const char* name)
			: Width(width), Height(height) {};

		int Width = 1600;
		int Height = 900;
		const char* Name = "Frostbyte Application";
	};

	class IWindow
	{
	public:
		bool Init(const WindowConfig& config);
		void Shutdown();
		void Update();

		static IWindow* Create();
	protected:
		virtual bool OnInit(const WindowConfig& config) = 0;
		virtual void OnShutdown() = 0;
		virtual void OnUpdate() = 0;

		void* m_WindowData = nullptr;
	};
}