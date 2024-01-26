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

		Window();
		Window(size_t _width, size_t _height);
		Window(size_t _width, size_t _height, std::string _windowName);
		Window(size_t _width, size_t _height, std::string _windowName, bool _resizable);

		~Window();

		static void testPrint();

		bool isOpen();
		void pollEvents();
		

	private:

		void initGLFWWindow();

	};

}


#endif