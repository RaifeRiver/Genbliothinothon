#include "Genbliothinothon.h"

namespace genbliothinothon {
	constexpr float VERTEX_BUFFER_DATA[] = {
			-1.0f, -1.0f, 0.0f,
		   1.0f, -1.0f, 0.0f,
		   0.0f,  1.0f, 0.0f
	};

	Genbliothinothon::Genbliothinothon() {
		mShader = std::make_unique<render::Shader>("../res/shaders/test.vert", "../res/shaders/test.frag");
		mVertexBuffer = std::make_unique<render::Buffer>(VERTEX_BUFFER_DATA, sizeof(VERTEX_BUFFER_DATA));

		mVAO.setAttribute(0, *mVertexBuffer, 3, GL_FLOAT);
	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			mVAO.bind();
			mShader->bind();
			mRenderContext.drawArrays(GL_TRIANGLES, 0, 3);

			mRenderContext.getTextRenderer().text(glm::vec2{0.2f, 0.2f}, 0.1f, "Hello World!", glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

			mRenderContext.getTextRenderer().render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}
}
