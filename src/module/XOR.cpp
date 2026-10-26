#include "XOR.h"

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	XOR::XOR(const glm::vec2& pos) : Module(pos, glm::vec2{200.0f, 118.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 118.0f}, "XOR"));
		addPart(std::make_unique<part::RemoveButton>(this));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, "Audio", "float",1));
		mAmplitude = &addPart(std::make_unique<part::Dial>(this, glm::vec2{10.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Amplitude", 0.0f, 1.0f, 1.0f));
		mValue = &addPart(std::make_unique<part::Dial>(this, glm::vec2{75.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Value", 0.1f, 10.0f, 1.0f));
	}

	XOR::XOR(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mAmplitude = reinterpret_cast<part::Dial*>(getParts()[3].get());
		mValue = reinterpret_cast<part::Dial*>(getParts()[4].get());
	}

	std::string XOR::generateGLSL(std::string) {
		return "XOR($Amplitude,$Value)";
	}
}
