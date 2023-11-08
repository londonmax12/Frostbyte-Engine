#include "Platform/IPlatform.h"
#include "Platform/WindowsImpl/WindowsPlatform.h"
#include "Platform/LinuxImpl/LinuxPlatform.h"

Frostbyte::IPlatform* Frostbyte::IPlatform::Init()
{
#ifdef FROSTBYTE_PLATFORM_WINDOWS
	m_Instance = (IPlatform*)(new WindowsPlatform());
#elif defined(FROSTBYTE_PLATFORM_LINUX)
	m_Instance = (IPlatform*)(new LinuxPlatform());
#else
	m_Instance = nullptr;
#endif
	return m_Instance;
}

void Frostbyte::IPlatform::Shutdown()
{
	if (m_Instance) {
		delete m_Instance;
		m_Instance = nullptr;
	}
}