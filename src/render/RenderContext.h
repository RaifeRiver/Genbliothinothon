#pragma once

#include "UIRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace genbliothinothon::render {
	class RenderContext {
	public:
		RenderContext();

		void clearColour(float r, float g, float b, float a = 1.0f);

		void setFBO(const Image& image) const;

		void clearFBO();

		void drawArrays(GLenum mode, GLint first, GLsizei count);

		[[nodiscard]] UIRenderer& getUIRenderer() const {
			return *mUIRenderer;
		}

		[[nodiscard]] glm::uvec2 getWindowSize() const {
			return mWindowSize;
		}

		[[nodiscard]] glm::vec2 getMouseMovement() const {
			return mMouseMovement;
		}

		[[nodiscard]] glm::vec2 getMousePos() const;

		[[nodiscard]] bool isButtonPressed(int button) const;

		void swapBuffers();

		[[nodiscard]] bool shouldClose() const;

		~RenderContext();

	private:
		GLFWwindow* mWindow;
		std::unique_ptr<UIRenderer> mUIRenderer;
		GLuint mFBO = 0;
		glm::uvec2 mWindowSize = {640, 480};
		glm::vec2 mMouseMovement = {0.0f, 0.0f};
	};
}
