#include "vgl/LogicalDevice.h"

vgl::LogicalDevice::LogicalDevice(std::shared_ptr<const VkInstance> _instance, const std::vector<const char*>& _deviceExtensions, std::shared_ptr<VkSurfaceKHR> _surface, std::unique_ptr<VkPhysicalDevice> _physicalDevice)
    : instance(_instance),
    deviceExtensions(_deviceExtensions),
    surface(_surface),
    physicalDevice(_physicalDevice)
{
    QueueFamilyIndices indices = this->findQueueFamilies(*this->physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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
        createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
        createInfo.ppEnabledLayerNames = this->validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    //Create logical device
    if (vkCreateDevice(this->physicalDevice, &createInfo, nullptr, &this->device) != VK_SUCCESS) {
        throw std::runtime_error("FAILED TO CREATE LOGICAL DEVICE");
    }

    vkGetDeviceQueue(this->device, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
    vkGetDeviceQueue(this->device, indices.presentFamily.value(), 0, &presentQueue);
	
}

QueueFamilyIndices vgl::LogicalDevice::findQueueFamilies(VkPhysicalDevice device){
    QueueFamilyIndices indices;

    //Retrieve the list of queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    //Find queue family that supports VK_QUEUE_GRAPHICS_BIT
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        //Check if can do graphcis
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        //Check if can render to surface
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        //Early exit if all queue families requires have been found
        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
