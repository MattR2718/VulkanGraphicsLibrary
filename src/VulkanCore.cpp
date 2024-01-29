#include "vgl/VulkanCore.h"

vgl::VulkanCore::VulkanCore(vgl::Window *_window) 
    : window(std::make_unique<vgl::Window>(*_window)),
    physicalDevice(
        (this->createInstance(), std::make_shared<const VkInstance>(this->instance)),
        this->deviceExtensions,
        (this->createInstance(), this->window->createVulkanSurface(this->instance), std::make_shared<VkSurfaceKHR>(this->window->surface))
    )
{
    this->setupDebugMessenger();

    std::cout << "CORE CREATED\n";
}

vgl::VulkanCore::~VulkanCore() {
    std::cout << "Destroying Vulkan Core\n";

    if (this->enableValidationLayers) {
        this->DestroyDebugUtilsMessengerEXT(this->instance, this->debugMessenger, nullptr);
    }

    //this->window->~Window();

    if (this->window->window) { this->window->~Window(); this->window.release(); }

    vkDestroyInstance(this->instance, nullptr);

    std::cout << "Destroyed Vulkan Core\n";
}






//Initialise Vulkan library by creating an instance
//The instance is the connection between the application and the Vulkan library
//Creating it  involves specifying details about the application to the driver
void vgl::VulkanCore::createInstance() {
    if (this->instance) { return; }
    //Check if validation layers have been requested and if so are available
    if (this->enableValidationLayers && !this->checkValidationLayerSupport()) {
        throw std::runtime_error("VALIDATION LAYERS REQUESTED, BUT NOT AVAILABLE");
    }
    
    //Initialise struct with information about applicaton
    //Technically optional but may provide useful information to the driver in order to optimise the application
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = this->window->windowName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    //Struct to tell the Vulkan Driver which global extensions and validation layers to use
    //Not optional
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //Vulkan is a platform agnostic API, meaning that you need an extension to interface with the window system
    //GLFW has a built-in function that returns the extension(s) it needs
    /*
    If Encountered VK_ERROR_INCOMPATIBLE_DRIVER
    https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Instance#:~:text=is%20created%20successfully.-,Encountered%20VK_ERROR_INCOMPATIBLE_DRIVER%3A,-If%20using%20MacOS
    */
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    //Include validation layer names if they are enabled
    if (this->enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
        createInfo.ppEnabledLayerNames = this->validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    //Add extensions
    auto extensions = this->getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    //Setup debug validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    //Create the Vulkan instance
    /*
    General pattern that object creation function parameters in Vulkan follows:
        - Pointer to struct with creation info
        - Pointer to custom allocator callbacks
        - Pointer to the variable that stores the handle to the new object
    */
    //Nearly all Vulkan functions return a value of the type VkResult that is either VK_SUCCESS or an error code
    if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) {
        throw std::runtime_error("FAILED TO CREATE INSTANCE");
    }
}



//Checks if all of the requested validation layers are available
bool vgl::VulkanCore::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : this->validationLayers) {
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





//Returns the required list of extensions based on whether validation layers are enabled or not
//The extensions specified by GLFW are always required, but the debug messenger extension is conditionally added
std::vector<const char*> vgl::VulkanCore::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (this->enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}






void vgl::VulkanCore::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    //Allows specification about the types of severities for the callback to be called for
    /*
    All options:
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT

    Using top 3 will mean receiving notifications about possible problems while leaving out verbose general debug info
    */
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    //Allows specification about the types of messages which the callback function is notified about
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    //Specifies the pointer to the callback function
    //Can optionally pass a pointer to the pUserData fueld which will be passed along to the callback function via the pUserData parameter.
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; //Optional
}









//Debug callback function
/*
The first parameter specifies the severity of the message, which is one of the following flags:
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes

The messageType parameter can have the following values:
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan

The pCallbackData parameter refers to a VkDebugUtilsMessengerCallbackDataEXT struct containing the details of the message itself, with the most important members being:
    pMessage: The debug message as a null-terminated string
    pObjects: Array of Vulkan object handles related to the message
    objectCount: Number of objects in array

The pUserData parameter contains a pointer that was specified during the setup of the callback and allows you to pass your own data to it

The callback returns a boolean that indicates if the Vulkan call that triggered the validation layer message should be aborted.
If the callback returns true, then the call is aborted wih the VK_ERROR_VALIDATION_FAILED_EXT error.
This is normally only used to test the validation layers themselves so should always return VK_FALSE

*/
VKAPI_ATTR VkBool32 VKAPI_CALL vgl::VulkanCore::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}



//Debug utils messenger helper functions
//Look up address of vkCreateDebugUtilsMessengerEXT function which is used to create the VkDebugUtilsMessengerEXT object
VkResult vgl::VulkanCore::CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

//Function to clean up the VkDebugUtilsMessengerEXT object
void vgl::VulkanCore::DestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}



void vgl::VulkanCore::setupDebugMessenger() {
    if (!this->enableValidationLayers) { return; }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    //Second to last parameter is again the optional allocator callbas that gets set to nullptr often
    if (this->CreateDebugUtilsMessengerEXT(this->instance, &createInfo, nullptr, &this->debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("FAILED TO SET UP DEBUG MESSENGER");
    }
}