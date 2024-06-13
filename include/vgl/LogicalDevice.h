#ifndef VGL_LOGICALDEVICE_H
#define VGL_LOGICALDEVICE_H

#include <set>

#include "vulkan/vulkan.hpp"

#include "vgl/QueueFamilyIndices.h"

namespace vgl {

	class LogicalDevice {

	public:

		VkDevice device = VK_NULL_HANDLE;

		LogicalDevice(std::shared_ptr<const VkInstance> _instance, const std::vector<const char*>& _deviceExtensions, std::shared_ptr<VkSurfaceKHR> _surface, std::unique_ptr<VkPhysicalDevice> _physicalDevice);

	private:

		//Vector to store all device extensions required
		const std::vector<const char*> deviceExtensions;

		//Store pointer to vulkan instance
		std::shared_ptr<const VkInstance> instance;

		//Store the surface created by the window
		std::shared_ptr<VkSurfaceKHR> surface;

		// Store the physical device
		std::unique_ptr<VkPhysicalDevice> physicalDevice = nullptr;

		/*
		Anything from drawing to uploading textures, requires commands to be submitted to a queue.
		There are different types of queues that originate from different queue families and each family of queues allows only a subset of commands.
			For example;
				There could be a queue family that only allows processing of compute commands
				There could be one that only allows memory transfer related commands.
		Need to check which queue families are supported by the device and which one of these supports the commands that are wanted to use.
		*/
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	};


}



#endif VGL_LOGICALDEVICE_H