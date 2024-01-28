#ifndef VGL_QUEUEFAMILYINDICES_H
#define VGL_QUEUEFAMILYINDICES_H

#include <optional>

namespace vgl {

    //Struct to contain the indicies for all queues required
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

}

#endif // !VGL_QUEUEFAMILYINDICES_H
