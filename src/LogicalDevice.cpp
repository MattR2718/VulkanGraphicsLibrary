#include "vgl/LogicalDevice.h"

vgl::LogicalDevice::LogicalDevice(std::shared_ptr<const VkInstance> _instance, const std::vector<const char*>& _deviceExtensions, std::shared_ptr<VkSurfaceKHR> _surface, std::shared_ptr<vgl::PhysicalDevice> _physicalDevice, bool _enableValidationLayers, std::shared_ptr<std::vector<const char*>> _validationLayers)
    : instance(_instance),
    deviceExtensions(_deviceExtensions),
    surface(_surface),
    physicalDevice(_physicalDevice),
    enableValidationLayers(_enableValidationLayers),
    validationLayers(_validationLayers)
{
    QueueFamilyIndices indices = this->physicalDevice->findQueueFamilies((*this->physicalDevice).physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    
    //TODO: Add back in when fix presentation family queue
    //std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        //Vulkan lets you assign priorities to queues to influence the scheduling of command buffer execution using floating point numbers between 0.0 and 1.0. 
        //This is required even if there is only a single queue
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    //Now need to specify the set of device features that'll be used.
    //These are the features that were queried for support with vkGetPhysicalDeviceFeatures, like geometry shaders.
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    //Enable sampple shading feature for the device
    //Enables multisampling of shaders at a performance cost
    deviceFeatures.sampleRateShading = VK_TRUE;

    //Create the logical device using the two structures above
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    //Pointer to queue creation struct
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    //Pointer to device features struct
    createInfo.pEnabledFeatures = &deviceFeatures;

    //Specify extensions and validation layers
    createInfo.enabledExtensionCount = static_cast<uint32_t>(this->deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = this->deviceExtensions.data();

    if (this->enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>((*this->validationLayers).size());
        createInfo.ppEnabledLayerNames = (*this->validationLayers).data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    //Create logical device
    if (vkCreateDevice((*this->physicalDevice).physicalDevice, &createInfo, nullptr, &this->logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("FAILED TO CREATE LOGICAL DEVICE");
    }

    vkGetDeviceQueue(this->logicalDevice, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
    //vkGetDeviceQueue(this->device, indices.presentFamily.value(), 0, &presentQueue);
	
}

vgl::LogicalDevice::~LogicalDevice(){
    if (logicalDevice != VK_NULL_HANDLE) {
        //vkDestroyDevice(logicalDevice, nullptr);
        
    }
}

