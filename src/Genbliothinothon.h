#pragma once

#include <memory>

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
		std::unique_ptr<render::Shader> mShader;
		std::unique_ptr<render::Buffer> mVertexBuffer;
	};
}
