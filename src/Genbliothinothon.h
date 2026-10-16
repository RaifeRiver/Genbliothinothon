#pragma once

#include "audio/AudioPlayer.h"
#include "render/RenderContext.h"

namespace genbliothinothon {
	class Genbliothinothon {
	public:
		Genbliothinothon();

		void run();

	private:
		render::RenderContext mRenderContext;
		audio::AudioPlayer mAudioPlayer;
	};
}
