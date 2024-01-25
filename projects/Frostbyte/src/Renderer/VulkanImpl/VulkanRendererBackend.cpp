#include "Renderer/VulkanImpl/VulkanRendererBackend.h"
#include "Renderer/VulkanImpl/VulkanUtils.h"
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

	if (CreateDebugUtilsMessengerEXT(m_Context.Instance, &messengerCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
		FROSTBYTE_ERROR("Failed to create Vulkan messenger");
		return false;
	}

	return true;
}
#endif

bool Frostbyte::VulkanRendererBackend::Init()
{
	FROSTBYTE_INFO("Creating Vulkan instance");
	if (!CreateInstance())
	{
		FROSTBYTE_FATAL("Failed to create vulkan instance");
		return false;
	}

#ifdef _DEBUG
	FROSTBYTE_INFO("Creating Vulkan messenger");
	if (!CreateMessenger()) {
		FROSTBYTE_FATAL("Failed to create vulkan messenger");
		return false;
	}
#endif

	FROSTBYTE_INFO("Initializing Vulkan device");
	m_Context.Device = new VulkanDevice();
	if (!m_Context.Device->Init(&m_Context))
	{
		FROSTBYTE_FATAL("Failed to initialize Vulkan device");
		return false;
	}

	FROSTBYTE_INFO("Creating Vulkan rendering surface");
	m_Context.Surface = IVulkanSurface::Create();
	if (!m_Context.Surface->Init()) {
		FROSTBYTE_FATAL("Failed to create Vulkan rendering surface");
		return false;
	}

	return true;
}

void Frostbyte::VulkanRendererBackend::Update()
{
}

void Frostbyte::VulkanRendererBackend::Shutdown()
{
	if (m_Context.ValidationLayersEnabled) {
		if (m_Context.ValidationLayers != nullptr) {
			free(m_Context.ValidationLayers);
		}
	}

	FROSTBYTE_INFO("Destroying Vulkan logical device");
	if (m_Context.Device) {
		m_Context.Device->Shutdown();
		delete m_Context.Device;
	}

#ifdef _DEBUG
	FROSTBYTE_INFO("Destroying Vulkan messenger");
	DestroyDebugUtilsMessengerEXT(m_Context.Instance, m_DebugMessenger, nullptr);
#endif

	FROSTBYTE_INFO("Destroying Vulkan instance");
	vkDestroyInstance(m_Context.Instance, nullptr);

	m_Context = VulkanContext();
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

	m_Context.ValidationLayersEnabled = false;
#ifdef _DEBUG
	m_Context.ValidationLayersEnabled = true;
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
	instanceCreateInfo.ppEnabledLayerNames = 0;

	m_Context.ValidationLayerCount = 1;
	m_Context.ValidationLayers = (const char**)malloc(m_Context.ValidationLayerCount * sizeof(const char*));
	m_Context.ValidationLayers[0] = "VK_LAYER_KHRONOS_validation";

	if (m_Context.ValidationLayers) {
		if (!HasValidationLayers()) {
			FROSTBYTE_ERROR("Failed to create Vulkan instance: Validation layers missing");
			return false;
		}
		instanceCreateInfo.enabledLayerCount = m_Context.ValidationLayerCount;
		instanceCreateInfo.ppEnabledLayerNames = m_Context.ValidationLayers;
	}
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Context.Instance);
	if (!VK_CHECK(result))
		return false;

	return true;
}

bool Frostbyte::VulkanRendererBackend::HasValidationLayers()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
	for (int i = 0; i < m_Context.ValidationLayerCount; i++) {
		const char* layerName = m_Context.ValidationLayers[i];
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
