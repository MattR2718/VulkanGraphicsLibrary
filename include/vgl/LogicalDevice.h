#ifndef VGL_LOGICALDEVICE_H
#define VGL_LOGICALDEVICE_H

#include <set>

#include "vulkan/vulkan.hpp"

#include "vgl/PhysicalDevice.h"
#include "vgl/QueueFamilyIndices.h"

namespace vgl {

	class LogicalDevice {

	public:

		VkDevice logicalDevice = VK_NULL_HANDLE;

		LogicalDevice() {};
		LogicalDevice(std::shared_ptr<const VkInstance> _instance, const std::vector<const char*>& _deviceExtensions, std::shared_ptr<VkSurfaceKHR> _surface, std::shared_ptr<vgl::PhysicalDevice> _physicalDevice, bool _enableValidationLayers, std::shared_ptr<std::vector<const char*>> _validationLayers);
		~LogicalDevice();


		//Implicitly define copy constructors
		LogicalDevice(const LogicalDevice&) = default;
		vgl::LogicalDevice& vgl::LogicalDevice::operator=(vgl::LogicalDevice& other) {
			if (this == &other) {
				return *this;
			}
			this->logicalDevice = other.logicalDevice;
			this->deviceExtensions = other.deviceExtensions;
			this->instance = other.instance;
			this->surface = other.surface;
			this->physicalDevice = other.physicalDevice;
			this->graphicsQueue = other.graphicsQueue;
			this->enableValidationLayers = other.enableValidationLayers;
			this->validationLayers = other.validationLayers;

			return *this;
		}

	private:

		//Vector to store all device extensions required
		std::vector<const char*> deviceExtensions;

		//Store pointer to vulkan instance
		std::shared_ptr<const VkInstance> instance;

		//Store the surface created by the window
		std::shared_ptr<VkSurfaceKHR> surface;

		// Store the physical device
		std::shared_ptr<vgl::PhysicalDevice> physicalDevice = nullptr;

		//Handle to graphics queue
		VkQueue graphicsQueue;

		bool enableValidationLayers = true;
		std::shared_ptr<std::vector<const char*>> validationLayers;

	};


}



#endif VGL_LOGICALDEVICE_H