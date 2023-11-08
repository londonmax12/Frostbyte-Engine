#pragma once

#include "Platform/PlatformDetection.h"
#include "Math/Vector.h"

namespace Frostbyte {
	struct WindowConfig {
		WindowConfig() = default;
		WindowConfig(int width, int height, const char* name)
			: WindowSize(width, height) {};

		VectorInt2 WindowSize = VectorInt2(1600, 900);
		const char* Name = "Frostbyte Application";
	};

	class IWindow
	{
	public:
		static IWindow* Create();
		
		bool Init(const WindowConfig& config);
		void Shutdown();
		void Update();
		
		VectorInt2 GetSize() { return m_Size; }

	protected:
		virtual bool OnInit(const WindowConfig& config) = 0;
		virtual void OnShutdown() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnResize(VectorInt2 size) = 0;

		VectorInt2 m_Size = 0;

		void* m_WindowData = nullptr;
	};
}