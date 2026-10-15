#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace genbliothinothon::render {
	class RenderContext {
	public:
		RenderContext();

		void clearColour(float r, float g, float b, float a = 1.0f);

		void swapBuffers() const;

		[[nodiscard]] bool shouldClose() const;

		~RenderContext();

	private:
		GLFWwindow* mWindow;
	};
}
