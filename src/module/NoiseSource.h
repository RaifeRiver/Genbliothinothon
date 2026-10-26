#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
	class NoiseSource : public Module {
	public:
		explicit NoiseSource(const glm::vec2& pos);

		NoiseSource(uint32_t version, util::FileIStream& stream);

		std::string generateGLSL(std::string send) override;

	private:
		part::Dial* mVolume;
		part::Dial* mLowPass;
		part::Dial* mHighPass;
	};
}
