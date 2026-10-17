#pragma once

#include "TextRenderer.h"
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

		[[nodiscard]] TextRenderer& getTextRenderer() const {
			return *mTextRenderer;
		}

		[[nodiscard]] glm::uvec2 getWindowSize() const {
			return mWindowSize;
		}

		void swapBuffers() const;

		[[nodiscard]] bool shouldClose() const;

		~RenderContext();

	private:
		GLFWwindow* mWindow;
		std::unique_ptr<TextRenderer> mTextRenderer;
		GLuint mFBO = 0;
		glm::uvec2 mWindowSize = {640, 480};
	};
}
