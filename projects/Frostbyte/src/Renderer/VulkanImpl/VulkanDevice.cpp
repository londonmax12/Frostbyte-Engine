#include "Renderer/VulkanImpl/VulkanDevice.h"
#include "Renderer/VulkanImpl/VulkanUtils.h"

#include "Logging/Logging.h"

bool Frostbyte::VulkanDevice::Init(const VulkanContext& context)
{
	FROSTBYTE_INFO("Picking physical device");
	if (!PickDevice(context))
		return false;

	FROSTBYTE_INFO("Creating logical device");
	if (!CreateLogicalDevice(context))
		return false;

	return true;
}

void Frostbyte::VulkanDevice::Shutdown()
{
	if (m_Device) {
		vkDestroyDevice(m_Device, 0);
	}
}

bool Frostbyte::VulkanDevice::PickDevice(const VulkanContext& context)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(context.Instance, &deviceCount, nullptr);

	if (!deviceCount) {
		FROSTBYTE_ERROR("Failed to find suitable graphics device for Vulkan");
		return VK_NULL_HANDLE;
	}

	VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(deviceCount * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(context.Instance, &deviceCount, devices);

	int bestScore = 0;

	for (int i = 0; i < deviceCount; i++) {
		VkPhysicalDevice device = devices[i];
		int deviceScore = 1;

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			deviceScore += 1000;

		deviceScore += deviceProperties.limits.maxImageDimension2D;

		if (!deviceFeatures.geometryShader)
			continue;

		if (!FindQueueFamilies(device))
			continue;

		FROSTBYTE_INFO("Found suitible device");
		FROSTBYTE_INFO("\t| Name: {}", deviceProperties.deviceName);
		FROSTBYTE_INFO("\t| Score: {}", deviceScore);

		if (deviceScore > bestScore)
			m_Physical = device;
	}

	free(devices);

	if (!m_Physical) {
		FROSTBYTE_ERROR("Failed to find suitable graphics device for Vulkan");
		return false;
	}

	return true;
}

bool Frostbyte::VulkanDevice::FindQueueFamilies(VkPhysicalDevice device)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

	bool foundGraphicsIndex = false;
	QueueFamilyIndices queueIndices{};

	for (int i = 0; i < queueFamilyCount; i++) {
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			queueIndices.GraphicsQueue = i;
			foundGraphicsIndex = true;
		}
	}

	free(queueFamilies);

	if (!foundGraphicsIndex)
		return false;

	m_QueueIndices = queueIndices;

	return true;
}

bool Frostbyte::VulkanDevice::CreateLogicalDevice(const VulkanContext& context)
{
	float queuePriority[1] = { 1.0f };

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = m_QueueIndices.GraphicsQueue;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = 0;
	deviceCreateInfo.enabledLayerCount = 0;

	if (context.ValidationLayersEnabled) {
		deviceCreateInfo.enabledLayerCount = context.ValidationLayerCount;
		deviceCreateInfo.ppEnabledLayerNames = context.ValidationLayers;
	}

	if (!VK_CHECK(vkCreateDevice(m_Physical, &deviceCreateInfo, 0, &m_Device)))
		return false;

	vkGetDeviceQueue(m_Device, m_QueueIndices.GraphicsQueue, 0, &m_GraphicsQueue);

	return true;
}
