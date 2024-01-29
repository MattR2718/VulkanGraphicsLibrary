#ifndef VGL_WINDOW_H
#define VGL_WINDOW_H

#include <iostream>

//Allow GLFW to include the Vulkan headers automatically
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace vgl {

	class Window {
	public:

		size_t width = 800;
		size_t height = 600;
		std::string windowName = "Vulkan App";

		bool resizable = true;

		GLFWwindow *window;

		//Surface
		/*
		Since Vulkan is a platform agnostic API, it can not interface directly with the window system on its own.
		Window System Integration extensions are required to establish a connection between Vulkan and the window system.
		The surface in the program will be backed by the window that was opened with GLFW

		The VK_KHR_surface extension is included in the list returned by glfwGetRequiredInstanceExtensions

		The window surface needs to be created right after the instance creation, because it can influence the physical device selection.

		Window surfaces are entirely optional component in Vulkan, off screen rendering is possible without any hacks (like creating invisible windows, which is required in OpenGL)
		*/
		VkSurfaceKHR surface;

		Window();
		Window(size_t _width, size_t _height);
		Window(size_t _width, size_t _height, std::string _windowName);
		Window(size_t _width, size_t _height, std::string _windowName, bool _resizable);

		~Window();

		static void testPrint();

		bool isOpen();
		void pollEvents();
		
		//Create surface for Vulkan
		void createVulkanSurface(VkInstance& instance);

	private:

		std::shared_ptr<VkInstance> instance;

		void initGLFWWindow();

	};

}


#endif