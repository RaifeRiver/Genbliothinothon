#include "WaveGenerator.h"

#include "part/Background.h"

namespace genbliothinothon::module {
	WaveGenerator::WaveGenerator(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 100.0f}) {
		addPart(std::make_unique<part::Background>(glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 100.0f}, "Wave generator"));
		mWaveType = &addPart(std::make_unique<part::Dropdown>(glm::vec2{3.0f, 24.0f}, glm::vec2{100.0f, 20.0f}, std::vector<std::string>{"Sine", "Ramp up", "Ramp down", "Triangle", "Square", "White noise"}));
	}
}
