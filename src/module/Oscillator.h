#pragma once

#include "Module.h"
#include "part/Dial.h"
#include "part/Dropdown.h"
#include "part/NoteDial.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	class Oscillator : public Module {
	public:
		explicit Oscillator(glm::vec2 pos);

		Oscillator(uint32_t version, util::FileIStream& stream);

		std::string generateGLSL(std::string send) override;

	private:
		part::Send* mAudio;
		part::Dropdown* mWaveType;
		part::NoteDial* mFrequency;
		part::Dial* mVolume;
		uint32_t mChannelCount = 1;

		void inputCallback();
	};
}
