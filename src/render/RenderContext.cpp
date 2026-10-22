#include "RenderContext.h"

#include <cstring>
#include <iostream>
#include <ranges>
#include <stdexcept>

#include "Genbliothinothon.h"

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

		glfwSetWindowUserPointer(mWindow, this);
		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, const int width, const int height) {
			auto* renderContext = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));
			glViewport(0, 0, width, height);
			renderContext->mWindowSize = glm::uvec2(width, height);
		});
		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, const double x, const double y) {
			auto* renderContext = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));
			static glm::vec2 lastPos = {0.0f, 0.0f};
			const glm::vec2 pos = {x, y};
			renderContext->mMouseMovement += pos - lastPos;
			lastPos = pos;
		});
		glfwSetCharCallback(mWindow, [](GLFWwindow*, const unsigned int c) {
			for (Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance(); const std::unique_ptr<module::Module>& module : genbliothinothon->getModuleManager().getModules() | std::views::values) {
				for (const std::unique_ptr<module::part::Part>& part : module->getParts()) {
					part->charPressed(static_cast<char>(c));
				}
			}
		});
		glfwSetKeyCallback(mWindow, [](GLFWwindow*, const int key, int, const int action, int) {
			if (action == GLFW_RELEASE) {
				return;
			}
			for (Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance(); const std::unique_ptr<module::Module>& module : genbliothinothon->getModuleManager().getModules() | std::views::values) {
				for (const std::unique_ptr<module::part::Part>& part : module->getParts()) {
					part->keyPressed(key);
				}
			}
		});

		int majorVersion = 0;
		int minorVersion = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		if (majorVersion >= 4 && minorVersion >= 3) {
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback([](GLenum, GLenum, const GLuint id, const GLenum severity, GLsizei, const GLchar* message, const void*) {
				const size_t messageLength = strlen(message);
				const std::string_view trimmedMessage(message, message[messageLength - 1] == '\n'? messageLength - 1 : messageLength);
				switch (severity) {
					case GL_DEBUG_SEVERITY_LOW:
					case GL_DEBUG_SEVERITY_MEDIUM:
						std::cout << "OpenGL Warning: id: " << id  << ", message: "<< trimmedMessage << std::endl;
						break;
					case GL_DEBUG_SEVERITY_HIGH:
						std::cerr << "OpenGL Error: id: " << id  << ", message: "<< trimmedMessage << std::endl;
						break;
					default:
						if (id == 131185) {
							break;
						}
						std::cout << "OpenGL Message: id: " << id  << ", message: "<< trimmedMessage << std::endl;
						break;
				}
			}, nullptr);
		}

		mUIRenderer = std::make_unique<UIRenderer>();

		glGenFramebuffers(1, &mFBO);
	}

	void RenderContext::clearColour(const float r, const float g, const float b, const float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderContext::setFBO(const Image& image) const {
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, image.getTexture(), 0);
	}

	void RenderContext::clearFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderContext::drawArrays(const GLenum mode, const GLint first, const GLsizei count) {
		glDrawArrays(mode, first, count);
	}

	glm::vec2 RenderContext::getMousePos() const {
		double x;
		double y;
		glfwGetCursorPos(mWindow, &x, &y);
		return glm::vec2{x, y};
	}

	bool RenderContext::isButtonPressed(const int button) const {
		return glfwGetMouseButton(mWindow, button) == GLFW_PRESS;
	}

	void RenderContext::swapBuffers() {
		mMouseMovement = glm::vec2{0.0f, 0.0f};
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	bool RenderContext::shouldClose() const {
		return glfwWindowShouldClose(mWindow);
	}

	RenderContext::~RenderContext() {
		mUIRenderer = nullptr;
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
}
