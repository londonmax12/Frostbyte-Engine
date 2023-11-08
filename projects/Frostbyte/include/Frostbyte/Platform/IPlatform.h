#pragma once

#include "Platform/PlatformDetection.h"

#include <string>

namespace Frostbyte {
	class IPlatform
	{
	public:
		virtual void GetVulkanExtentions(int* outCount, const char*** outNames) = 0;
		virtual void Print(std::string msg) = 0;
		virtual void PrintWarning(std::string msg) = 0;
		virtual void PrintError(std::string msg) = 0;

		static IPlatform* Init();
		static IPlatform* GetInstance() { return m_Instance; };
		static void Shutdown();

	private:
		inline static IPlatform* m_Instance;
	};
}