#pragma once
#include "Platform/IPlatform.h"

#ifdef FROSTBYTE_PLATFORM_LINUX

namespace Frostbyte {
	class LinuxPlatform : public IPlatform {
	public:
		void GetVulkanExtentions(int* outCount, const char*** outNames) override;
	};
}
#endif