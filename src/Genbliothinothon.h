#pragma once

#include "render/Buffer.h"
#include "render/RenderContext.h"
#include "render/Shader.h"
#include "render/VAO.h"

namespace genbliothinothon {
	class Genbliothinothon {
	public:
		Genbliothinothon();

		bool shouldClose() const;
		void renderFrame();

	private:
		render::RenderContext mRenderContext;
		render::VAO mVAO;
		render::Buffer mVertexBuffer;
		render::Shader mShader;
	};
}
