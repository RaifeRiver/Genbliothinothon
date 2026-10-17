#pragma once

#include "Module.h"
#include "part/Dropdown.h"

namespace genbliothinothon::module {
	class WaveGenerator : public Module {
	public:
		explicit WaveGenerator(glm::vec2 pos);

		std::string generateGLSL(const std::string& inputName) override;

	private:
		part::Dropdown* mWaveType;
	};
}
