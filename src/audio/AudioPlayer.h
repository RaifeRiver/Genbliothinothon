#pragma once

#include "portaudio.h"

#include "module/ModuleManager.h"
#include "render/Image.h"
#include "render/RenderContext.h"
#include "render/VAO.h"

namespace genbliothinothon::audio {
	static constexpr bool ENABLE_SCOPE = false;

	class AudioPlayer {
	public:
		AudioPlayer();

		void rebuildShader(module::ModuleManager& moduleManager);

		void update(render::RenderContext& renderContext);

		~AudioPlayer();

	private:
		render::VAO mVAO;
		std::unique_ptr<render::Shader> mShader;
		std::unique_ptr<render::Buffer> mVertexBuffer;
		std::unique_ptr<render::Image> mOutputImage;
		std::unique_ptr<render::Shader> mOscilloscopeShader;
		GLsync mFence = nullptr;
		bool mFirst = true;
		PaStream* mStream = nullptr;
		uint32_t mNextSampleBase = 0;
	};
}
