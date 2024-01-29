#include "vgl/Window.h"

//Constructors
vgl::Window::Window(){
	this->initGLFWWindow();
}
vgl::Window::Window(size_t _width, size_t _height) : width(_width), height(_height) {
	this->initGLFWWindow();
}
vgl::Window::Window(size_t _width, size_t _height, std::string _windowName) : width(_width), height(_height), windowName(_windowName) {
	this->initGLFWWindow();
}
vgl::Window::Window(size_t _width, size_t _height, std::string _windowName, bool _resizable) : width(_width), height(_height), windowName(_windowName), resizable(_resizable) {
	this->initGLFWWindow();
}


//Destroctor
vgl::Window::~Window() {
	//Vulkan core destructor calls the window destructor since the window requires the core to be destroyed
	//Shared pointer requires window is cleaned up before core
	//Window destructor can be called twice, when the core is destroyed/goes out of scope and when the window goes out of scope
	//Have to check if inctance and surface have not been destroyed to avoid errors
	if (this->instance && this->surface) {
		vkDestroySurfaceKHR(*this->instance, this->surface, nullptr);
	}
	if (this->window) {
		glfwDestroyWindow(this->window);
	}
	
	glfwTerminate();
}

void vgl::Window::testPrint(){
	std::cout << "IN WINDOW\n";
}


bool vgl::Window::isOpen() {
	return !glfwWindowShouldClose(this->window);
}

void vgl::Window::pollEvents() {
	glfwPollEvents();
}


//Create Vulkan surface
void vgl::Window::createVulkanSurface(VkInstance& instance) {
	this->instance = std::make_shared<VkInstance>(instance);
	if (glfwCreateWindowSurface(instance, this->window, nullptr, &this->surface) != VK_SUCCESS) {
		throw std::runtime_error("FAILED TO CREATE WINDOW SURFACE");
	}
}


void vgl::Window::initGLFWWindow() {
	//Initialise GLFW Library
	glfwInit();

	//GLFW was originally designed to create an OpenGL context so tell it not to create an OpenGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//Set whether window can resize
	if (this->resizable) {
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	}
	else {
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	//Create the window
	//Parameters: Width, Height, Window Title, Monitor Specification, Something to do with OpenGL
	this->window = glfwCreateWindow(this->width, this->height, this->windowName.c_str(), nullptr, nullptr);

	glfwSetWindowUserPointer(this->window, this);

	//TODO TODO TODO
	//Sort this out later
	//glfwSetFramebufferSizeCallback(this->window, this->framebufferResizeCallback);

}