#pragma once

#include "Module.h"
#include "part/Dial.h"
#include "part/Dropdown.h"

namespace genbliothinothon::module {
	class WaveGenerator : public Module {
	public:
		explicit WaveGenerator(glm::vec2 pos);

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		part::Dropdown* mWaveType;
		part::Dial* mFrequency;
		part::Dial* mVolume;
	};
}
