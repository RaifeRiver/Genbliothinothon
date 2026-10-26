#include "NoiseSource.h"

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	NoiseSource::NoiseSource(const glm::vec2& pos) : Module(pos, glm::vec2{200.0f, 118.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 118.0f}, "Noise Source"));
		addPart(std::make_unique<part::RemoveButton>(this));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, "Audio", "float"));
		mVolume = &addPart(std::make_unique<part::Dial>(this, glm::vec2{10.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Volume", 0.0f, 1.0f, 1.0f));
		mLowPass = &addPart(std::make_unique<part::Dial>(this, glm::vec2{75.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Low Pass", 10.0f, 20000.0f, 20000.0f));
		mHighPass = &addPart(std::make_unique<part::Dial>(this, glm::vec2{140.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "High Pass", 10.0f, 20000.0f, 10.0f));
	}

	NoiseSource::NoiseSource(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mVolume = reinterpret_cast<part::Dial*>(getParts()[3].get());
		mLowPass = reinterpret_cast<part::Dial*>(getParts()[4].get());
		mHighPass = reinterpret_cast<part::Dial*>(getParts()[5].get());
	}

	std::string NoiseSource::generateGLSL(std::string send) {
		return "noise($Volume,$Low Pass,$High Pass)";
	}
}
