#ifndef VGL_VULKANCORE_H
#define VGL_VULKANCORE_H

#include <iostream>

#include "vulkan/vulkan.hpp"
#include "vgl/window.h"

namespace vgl {

	class VulkanCore {

	public:

		std::unique_ptr<vgl::Window> window;

		VulkanCore(std::unique_ptr<vgl::Window> _window);
        ~VulkanCore();


	private:

		VkInstance instance;

        VkDebugUtilsMessengerEXT debugMessenger;




        //Validation Layers
        //Specific validation layers requested
        const std::vector<const char*> validationLayers = {
            //Built in Khronos validation layers
            "VK_LAYER_KHRONOS_validation"
        };
        //If 'Not Debug' then disable the validation layers
        #ifdef NDEBUG
                const bool enableValidationLayers = false;
        #else
                const bool enableValidationLayers = true;
        #endif

		void createInstance();
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);


        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);

        void setupDebugMessenger();
	};



}



#endif