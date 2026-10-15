#include "RenderContext.h"

#include <iostream>
#include <stdexcept>

namespace genbliothinothon::render {
	RenderContext::RenderContext() {
		glfwInit();
		glfwSetErrorCallback([](int, const char* description) {
			std::cerr << "GLFW error: " << description << std::endl;
		});

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		mWindow = glfwCreateWindow(640, 480, "Genbliothinothon", nullptr, nullptr);
		if (!mWindow) {
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(mWindow);
	}

	void RenderContext::swapBuffers() const {
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	bool RenderContext::shouldClose() const {
		return glfwWindowShouldClose(mWindow);
	}

	RenderContext::~RenderContext() {
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
}
