#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace genbliothinothon::render {
	class RenderContext {
	public:
		RenderContext();

		void swapBuffers() const;

		[[nodiscard]] bool shouldClose() const;

		~RenderContext();

	private:
		GLFWwindow* mWindow;
	};
}
