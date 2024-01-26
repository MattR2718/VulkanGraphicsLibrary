#include "vgl/window.h"

int main() {
	vgl::Window window(1920, 1080, "Window Title");

	while (window.isOpen()) {
		window.pollEvents();
	}
}