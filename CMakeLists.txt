# CMakeList.txt : CMake project for VulkanGraphicsLibrary, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (vgl VERSION 0.0.1 DESCRIPTION "Vulkan Graphics Library")

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME} 
	PRIVATE 
		src/window.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/vgl>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(public_headers 
    include
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

#include_directories(src)

add_subdirectory(examples)
add_subdirectory(src)
add_subdirectory(include)

# Add source to this project's executable.
add_executable (TestHello "VulkanGraphicsLibrary.cpp" "VulkanGraphicsLibrary.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestHello PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
