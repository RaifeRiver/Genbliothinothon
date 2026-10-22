#pragma once

#include "audio/AudioPlayer.h"
#include "module/ModuleManager.h"
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

		[[nodiscard]] module::ModuleManager& getModuleManager() {
			return mModuleManager;
		}

		[[nodiscard]] glm::vec2& getCameraPos() {
			return mCameraPos;
		}

		[[nodiscard]] static Genbliothinothon* getInstance() {
			return sInstance;
		}

		~Genbliothinothon();

	private:
		inline static Genbliothinothon* sInstance;

		render::RenderContext mRenderContext;
		audio::AudioPlayer mAudioPlayer;
		module::ModuleManager mModuleManager;
		glm::vec2 mCameraPos = {0.0f, 0.0f};
	};
}
