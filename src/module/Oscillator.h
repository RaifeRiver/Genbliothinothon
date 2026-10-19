#pragma once

#include "Module.h"
#include "part/Dial.h"
#include "part/Dropdown.h"
#include "part/NoteDial.h"

namespace genbliothinothon::module {
	class Oscillator : public Module {
	public:
		explicit Oscillator(glm::vec2 pos);

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		part::Dropdown* mWaveType;
		part::NoteDial* mFrequency;
		part::Dial* mVolume;
	};
}
