#ifndef VGL_PHYSICALDEVICE_H
#define VGL_PHYSICALDEVICE_H

#include "vulkan/vulkan.hpp"

#include <set>
#include <iostream>

#include "vgl/QueueFamilyIndices.h"
#include "vgl/SwapChainSupportDetails.h"

namespace vgl {

	class PhysicalDevice {

	public:

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		//Store number of samples used for MSAA multisampling
		//By default use a single sample per pixel (equivalent to no multisampling)
		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

		PhysicalDevice() {};
		PhysicalDevice(std::shared_ptr<const VkInstance> _instance, const std::vector<const char*>& _deviceExtensions, std::shared_ptr<VkSurfaceKHR> _surface);

		//Implicitly define copy constructors
		PhysicalDevice(const PhysicalDevice&) = default;
		vgl::PhysicalDevice& vgl::PhysicalDevice::operator=(vgl::PhysicalDevice& d) { return d; }

		void setInstance(std::shared_ptr<const VkInstance> _instance);

		void setSurface(std::shared_ptr<VkSurfaceKHR> _surface);

	private:

		//Vector to store all device extensions required
		const std::vector<const char*> deviceExtensions;

		//Store pointer to vulkan instance
		std::shared_ptr<const VkInstance> instance;

		//Store the surface created by the window
		std::shared_ptr<VkSurfaceKHR> surface;

		//Check whether a given physical device is suitable
		//Based on certain parameters required in the function
		bool isDeviceSuitable(const VkPhysicalDevice& device);

		//Check if the device supports all required extensions
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);

		/*
		Anything from drawing to uploading textures, requires commands to be submitted to a queue.
		There are different types of queues that originate from different queue families and each family of queues allows only a subset of commands.
			For example;
				There could be a queue family that only allows processing of compute commands
				There could be one that only allows memory transfer related commands.
		Need to check which queue families are supported by the device and which one of these supports the commands that are wanted to use.
		*/
		vgl::QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

		//Populate SwapChainSupportDetails struct
		vgl::SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device);

		//Get maximum number of samples supported by the device
		VkSampleCountFlagBits getMaxUsableSampleCount();

	};


}




#endif VGL_PHYSICALDEVICE_H