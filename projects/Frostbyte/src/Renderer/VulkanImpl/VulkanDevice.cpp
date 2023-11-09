#include "Renderer/VulkanImpl/VulkanDevice.h"

#include "Logging/Logging.h"

bool Frostbyte::VulkanDevice::Init(VulkanContext* context)
{
	if (!PickDevice(context))
		return false;

	return true;
}

void Frostbyte::VulkanDevice::Shutdown()
{
}

bool Frostbyte::VulkanDevice::PickDevice(VulkanContext* context)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(context->Instance, &deviceCount, nullptr);

	if (!deviceCount) {
		FROSTBYTE_ERROR("Failed to find suitable graphics device for Vulkan");
		return VK_NULL_HANDLE;
	}

	VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(deviceCount * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(context->Instance, &deviceCount, devices);

	int bestDevice = 0;
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

		FROSTBYTE_INFO("Finding queue family indices");
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
