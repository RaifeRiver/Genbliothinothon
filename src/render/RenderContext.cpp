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
		glfwWindowHint(GLFW_SAMPLES, 4);

		mWindow = glfwCreateWindow(640, 480, "Genbliothinothon", nullptr, nullptr);
		if (!mWindow) {
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(mWindow);
		if (!gladLoadGL()) {
			throw std::runtime_error("Failed to initialize OpenGL");
		}

		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow*, const int width, const int height) {
			glViewport(0, 0, width, height);
		});
	}

	void RenderContext::clearColour(const float r, const float g, const float b, const float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderContext::drawArrays(const GLenum mode, const GLint first, const GLsizei count) {
		glDrawArrays(mode, first, count);
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
