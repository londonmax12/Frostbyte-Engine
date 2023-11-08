#include "Renderer/VulkanImpl/VulkanRendererBackend.h"
#include "Logging/Logging.h"
#include "Platform/IPlatform.h"

#include <cstring>

#ifdef _DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData) {
	switch (severity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		FROSTBYTE_WARNING("Vulkan Warning: {}", callbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		FROSTBYTE_ERROR("Vulkan Error: {}", callbackData->pMessage);
		break;
	default:
		FROSTBYTE_INFO("Vulkan: {}", callbackData->pMessage);
		break;
	}
	return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, createInfo, allocator, debugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, allocator);
	}
}
bool Frostbyte::VulkanRendererBackend::CreateMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
	messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	messengerCreateInfo.pfnUserCallback = DebugCallback;
	messengerCreateInfo.pUserData = nullptr;

	if (CreateDebugUtilsMessengerEXT(m_Instance, &messengerCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
		FROSTBYTE_ERROR("Failed to create Vulkan messenger");
		return false;
	}

	return true;
}
#endif

bool Frostbyte::VulkanRendererBackend::Init()
{
	FROSTBYTE_INFO("Creating Vulkan instance");
	CreateInstance();

#ifdef _DEBUG
	FROSTBYTE_INFO("Creating Vulkan messenger");
	CreateMessenger();
#endif

	return true;
}

void Frostbyte::VulkanRendererBackend::Update()
{
}

void Frostbyte::VulkanRendererBackend::Shutdown()
{
#ifdef _DEBUG
	FROSTBYTE_INFO("Destroying Vulkan messenger");
	DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
#endif

	FROSTBYTE_INFO("Destroying Vulkan instance");
	vkDestroyInstance(m_Instance, nullptr);
}

bool Frostbyte::VulkanRendererBackend::CreateInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Frostbyte Application";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
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
#ifdef _DEBUG
	reqExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	for (int i = 0; i < platformExtensionCount; i++) {
		reqExtensions.push_back(platformExtensions[i]);
	}

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	for (const auto& reqExtension : reqExtensions) {
		bool found = false;
		for (const auto& extension : extensions)
			if (strcmp(extension.extensionName, reqExtension)) {
				found = true;
				break;
			}
		if (!found)
		{
			FROSTBYTE_ERROR("Failed to create Vulkan instance: {} required but not found", reqExtension);
			return false;
		}
	}

	instanceCreateInfo.enabledExtensionCount = (uint32_t)reqExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = reqExtensions.data();
	instanceCreateInfo.enabledLayerCount = 0;

#ifdef _DEBUG
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	if (!HasValidationLayers(validationLayers)) {
		FROSTBYTE_ERROR("Failed to create Vulkan instance: Validation layers missing");
		return false;
	}
	instanceCreateInfo.enabledLayerCount = (uint32_t)validationLayers.size();
	instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#endif

	if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) != VK_SUCCESS) {
		FROSTBYTE_ERROR("Failed to create Vulkan instance");
		return false;
	}
	return false;
}

bool Frostbyte::VulkanRendererBackend::HasValidationLayers(const std::vector<const char*> validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}
