#include "vgl/PhysicalDevice.h"
#include "vgl/PhysicalDevice.h"


vgl::PhysicalDevice::PhysicalDevice(std::shared_ptr<const VkInstance> _instance, const std::vector<const char*>& _deviceExtensions, std::shared_ptr<VkSurfaceKHR> _surface)
    : instance(_instance),
    deviceExtensions(_deviceExtensions),
    surface(_surface)
{
    //Count all possible physical devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*this->instance, &deviceCount, nullptr);

    //If no devices available then exit
    if (deviceCount == 0) {
        throw std::runtime_error("FAILED TO FIND GPUs WITH VULKAN SUPPORT");
    }

    //Create a vector to store all possible physical devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*this->instance, &deviceCount, devices.data());

    //Loop over devices and check if they are suitable
    for (const auto& device : devices) {
        if (this->isDeviceSuitable(device)) {
            this->physicalDevice = device;
            this->msaaSamples = this->getMaxUsableSampleCount();
            break;
        }
    }

    //Check if a suitable device was found
    if (this->physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("UNABLE TO FIND A SUITABLE GPU");
    }
}

bool vgl::PhysicalDevice::isDeviceSuitable(const VkPhysicalDevice& device){
    //Get basic device properties
    //e.g. name, type, supported vulkan version
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    //Support for optional features
    //e.g. texture compression, 64 bit floats, multi viewport rendering (useful for VR)
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    //Example check for dedicated graphics cards that support geometry shaders
    //return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
    //        && deviceFeatures.geometryShader;

    //Get whether deivce supports the queue families required
    QueueFamilyIndices indices = this->findQueueFamilies(device);

    //Check whether the device supports extensions
    bool extensionsSupported = this->checkDeviceExtensionSupport(device);

    //Check swap chain availabilities
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        vgl::SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool vgl::PhysicalDevice::checkDeviceExtensionSupport(const VkPhysicalDevice& device){
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(this->deviceExtensions.begin(), this->deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

vgl::QueueFamilyIndices vgl::PhysicalDevice::findQueueFamilies(const VkPhysicalDevice& device){
    vgl::QueueFamilyIndices indices;

    //Retrieve the list of queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    //Find queue family that supports VK_QUEUE_GRAPHICS_BIT
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        //Check if can do graphics
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        //Check if can render to surface
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *this->surface, &presentSupport);
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

vgl::SwapChainSupportDetails vgl::PhysicalDevice::querySwapChainSupport(const VkPhysicalDevice& device){
    vgl::SwapChainSupportDetails details;

    //Query surface capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *this->surface, &details.capabilities);

    //Query surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, *this->surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, *this->surface, &formatCount, details.formats.data());
    }

    //Query supported presentation modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, *this->surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, *this->surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSampleCountFlagBits vgl::PhysicalDevice::getMaxUsableSampleCount(){
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(this->physicalDevice, &physicalDeviceProperties);

    //Get max number of samples for frame buffer colour and depth buffer
    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    //Find maximum number of samples supported by both colour and depth buffer
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}
