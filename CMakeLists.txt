# CMakeList.txt : CMake project for VulkanGraphicsLibrary, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VulkanGraphicsLibrary" VERSION 0.0.1 DESCRIPTION "Vulkan Graphics Library")

include_directories(src)

add_subdirectory(examples)
add_subdirectory(src)

# Add source to this project's executable.
add_executable (TestHello "VulkanGraphicsLibrary.cpp" "VulkanGraphicsLibrary.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestHello PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
