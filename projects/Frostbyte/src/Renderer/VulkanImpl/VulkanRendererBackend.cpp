#include "Renderer/VulkanImpl/VulkanRendererBackend.h"
#include "Logging/Logging.h"
#include "Platform/IPlatform.h"

#include <vector>

bool Frostbyte::VulkanRendererBackend::Init()
{
	FROSTBYTE_INFO("Initializing Vulkan renderer");

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Frostbyte Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0, 0);
	appInfo.pEngineName = "Frostbyte Engine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	
	int platformExtensionCount = 0;
	const char** platformExtensions;
	IPlatform::GetInstance()->GetVulkanExtentions(&platformExtensionCount, &platformExtensions);

	std::vector<const char*> reqExtensions = std::vector<const char*>({ VK_KHR_SURFACE_EXTENSION_NAME });
	for (int i = 0; i < platformExtensionCount; i++) {
		reqExtensions.push_back(platformExtensions[i]);
	}
	instanceCreateInfo.enabledExtensionCount = reqExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = reqExtensions.data();
	instanceCreateInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) != VK_SUCCESS) {
		FROSTBYTE_ERROR("Failed to create Vulkan instance");
		return false;
	}

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	for (const auto& reqExtension : reqExtensions) {
		bool found = false;
		for (const auto& extension : extensions) {
			if (strcmp(extension.extensionName, reqExtension)) {
				found = true;
				break;
			}
		}
		if (!found)
		{
			FROSTBYTE_ERROR("Failed to create Vulkan instance: {} required but not found", reqExtension);
			return false;
		}
	}

	return true;
}

void Frostbyte::VulkanRendererBackend::Update()
{
}

void Frostbyte::VulkanRendererBackend::Shutdown()
{
	FROSTBYTE_INFO("Destroying Vulkan instance");
	vkDestroyInstance(m_Instance, nullptr);
}

bool Frostbyte::VulkanRendererBackend::CreateInstance()
{
	return false;
}