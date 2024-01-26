#include "vgl/window.h"
#include "vgl/VulkanCore.h"

int main() {
	vgl::Window window(1920, 1080, "Window Title");

	vgl::VulkanCore vk(std::make_unique<vgl::Window>(window));

	while (window.isOpen()) {
		window.pollEvents();
	}
}