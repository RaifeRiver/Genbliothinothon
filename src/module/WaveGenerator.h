#pragma once

#include "Module.h"

namespace genbliothinothon::module {
	enum class WaveType {
		SINE,
		RAMP_UP,
		RAMP_DOWN,
		TRIANGLE,
		SQUARE,
		WHITE_NOISE
	};

	class WaveGenerator : public Module {
	public:
		explicit WaveGenerator(glm::vec2 pos);

	private:
		WaveType mWaveType = WaveType::SINE;
	};
}
