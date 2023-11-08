#pragma once
#include "Platform/IPlatform.h"

#ifdef FROSTBYTE_PLATFORM_LINUX

namespace Frostbyte {
	class LinuxPlatform : public IPlatform {
	public:
		void GetVulkanExtentions(int* outCount, const char*** outNames) override;
		void Print(std::string msg) override;
		void PrintWarning(std::string msg) override;
		void PrintError(std::string msg) override;
	};
}
#endif