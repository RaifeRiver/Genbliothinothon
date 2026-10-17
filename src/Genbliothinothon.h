#pragma once

#include "audio/AudioPlayer.h"
#include "module/Module.h"
#include "render/RenderContext.h"

namespace genbliothinothon {
	class Genbliothinothon {
	public:
		Genbliothinothon();

		void run();

	private:
		render::RenderContext mRenderContext;
		audio::AudioPlayer mAudioPlayer;
		std::vector<std::unique_ptr<module::Module>> mModules;
		glm::vec2 mCameraPos = {0.0f, 0.0f};
	};
}
