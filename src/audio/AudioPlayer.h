#pragma once

#include "portaudio.h"

#include "render/Image.h"
#include "render/RenderContext.h"
#include "render/VAO.h"

namespace genbliothinothon::audio {
	class AudioPlayer {
	public:
		AudioPlayer();

		void update(render::RenderContext& renderContext);

		~AudioPlayer();

	private:
		render::VAO mVAO;
		std::unique_ptr<render::Shader> mShader;
		std::unique_ptr<render::Buffer> mVertexBuffer;
		std::unique_ptr<render::Image> mOutputImage;
		GLsync mFence = nullptr;
		bool mFirst = true;
		PaStream* mStream = nullptr;
		uint32_t mNextSampleBase = 0;
	};
}
