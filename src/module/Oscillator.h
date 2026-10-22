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

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		part::Send* mAudio;
		part::Dropdown* mWaveType;
		part::NoteDial* mFrequency;
		part::Dial* mVolume;
	};
}
