#include "vgl/Window.h"
#include "vgl/VulkanCore.h"

int main() {
	vgl::Window window(1920, 1080, "Window Title");

	vgl::VulkanCore vk(&window);
	 
	while (window.isOpen()) {
		window.pollEvents();
	}
}