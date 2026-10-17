#pragma once

#include "audio/AudioPlayer.h"
#include "module/Module.h"
#include "render/RenderContext.h"

namespace genbliothinothon {
	class Genbliothinothon {
	public:
		Genbliothinothon();

		void run();

		[[nodiscard]] render::RenderContext& getRenderContext() {
			return mRenderContext;
		}

		[[nodiscard]] audio::AudioPlayer& getAudioPlayer() {
			return mAudioPlayer;
		}

		[[nodiscard]] std::vector<std::unique_ptr<module::Module>>& getModules() {
			return mModules;
		}

		[[nodiscard]] glm::vec2 getCameraPos() const {
			return mCameraPos;
		}

		[[nodiscard]] static Genbliothinothon* getInstance() {
			return sInstance;
		}

	private:
		inline static Genbliothinothon* sInstance;

		render::RenderContext mRenderContext;
		audio::AudioPlayer mAudioPlayer;
		std::vector<std::unique_ptr<module::Module>> mModules;
		glm::vec2 mCameraPos = {0.0f, 0.0f};
	};
}
