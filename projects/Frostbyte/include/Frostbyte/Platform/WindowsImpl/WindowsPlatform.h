#pragma once
#include "Platform/IPlatform.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS

namespace Frostbyte {
	class WindowsPlatform : public IPlatform {
	public:
		void GetVulkanExtentions(int* outCount, const char*** outNames) override;
	};
}
#endif