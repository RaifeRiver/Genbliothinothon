#include "Tremlo.h"

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	Tremlo::Tremlo(const glm::vec2& pos) : Module(pos, glm::vec2{265.0f, 118.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{265.0f, 118.0f}, "Tremlo"));
		addPart(std::make_unique<part::RemoveButton>(this));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{259.0f, 20.0f}, "Left", "float", 1));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{259.0f, 20.0f}, "Right", "float", 1));
		mAmplitude = &addPart(std::make_unique<part::Dial>(this, glm::vec2{10.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Amplitude", 0.0f, 1.0f, 1.0f));
		mPeriod = &addPart(std::make_unique<part::Dial>(this, glm::vec2{75.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Period", 0.1f, 10.0f, 1.0f));
		mGain = &addPart(std::make_unique<part::Dial>(this, glm::vec2{140.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Gain", 0.1f, 10.0f, 1.0f));
		mSmooth = &addPart(std::make_unique<part::Dial>(this, glm::vec2{205.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Smooth", 0.1f, 10.0f, 1.0f));
	}

	Tremlo::Tremlo(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mAmplitude = reinterpret_cast<part::Dial*>(getParts()[4].get());
		mPeriod = reinterpret_cast<part::Dial*>(getParts()[5].get());
		mGain = reinterpret_cast<part::Dial*>(getParts()[6].get());
		mSmooth = reinterpret_cast<part::Dial*>(getParts()[7].get());
	}

	std::string Tremlo::generateGLSL(const std::string send) {
		return std::string("Tremlo($Sample,$Amplitude,$Period,$Gain,$Smooth).") + (send == "Left" ? "x" : "y");
	}
}
