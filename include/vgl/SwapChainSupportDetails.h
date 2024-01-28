#ifndef VGL_SWAPCHAINSUPPORTDETAILS_H
#define VGL_SWAPCHAINSUPPORTDETAILS_H

#include "vulkan/vulkan.hpp"

namespace vgl {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

}

#endif // !VGL_SWAPCHAINSUPPORTDETAILS_H
