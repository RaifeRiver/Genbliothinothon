#include "WaveGenerator.h"

#include "part/Background.h"

namespace genbliothinothon::module {
	WaveGenerator::WaveGenerator(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 100.0f}) {
		addPart(std::make_unique<part::Background>(glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 100.0f}, "Wave generator"));
	}
}
