#pragma once

#include "Module.h"
#include "part/Dropdown.h"

namespace genbliothinothon::module {
	class WaveGenerator : public Module {
	public:
		explicit WaveGenerator(glm::vec2 pos);

	private:
		part::Dropdown* mWaveType;
	};
}
