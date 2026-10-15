#include "Genbliothinothon.h"

namespace genbliothinothon {
	constexpr float VERTEX_BUFFER_DATA[] = {
			-1.0f, -1.0f, 0.0f,
		   1.0f, -1.0f, 0.0f,
		   0.0f,  1.0f, 0.0f
	};

	Genbliothinothon::Genbliothinothon() : mVertexBuffer(VERTEX_BUFFER_DATA, sizeof(VERTEX_BUFFER_DATA)), mShader("../res/shaders/test.vert", "../res/shaders/test.frag") {
		mVAO.setAttribute(0, mVertexBuffer, 3, GL_FLOAT);
	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			mVAO.bind();
			mShader.bind();
			mRenderContext.drawArrays(GL_TRIANGLES, 0, 3);

			mRenderContext.swapBuffers();
		}
	}
}
