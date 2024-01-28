#include "vgl/Window.h"

int main() {
	vgl::Window::testPrint();

	vgl::Window window(1920, 1080, "Example Window", true);

	while (window.isOpen()) {
		window.pollEvents();
	}


}