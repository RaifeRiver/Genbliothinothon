#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
	class NoiseSource : public Module {
	public:
		explicit NoiseSource(const glm::vec2& pos);

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		part::Dial* mVolume;
		part::Dial* mLowPass;
		part::Dial* mHighPass;
	};
}
