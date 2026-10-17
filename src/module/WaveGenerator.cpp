#include "WaveGenerator.h"

#include "part/Background.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	WaveGenerator::WaveGenerator(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 100.0f}) {
		addPart(std::make_unique<part::Background>(glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 100.0f}, "Wave generator"));
		addPart(std::make_unique<part::Send>(glm::vec2{3.0f, 46.0f}, glm::vec2{194.0f, 20.0f}));
		mWaveType = &addPart(std::make_unique<part::Dropdown>(glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, std::vector<std::string>{"Sine", "Ramp up", "Ramp down", "Triangle", "Square", "White noise"}));
	}

	std::string WaveGenerator::generateGLSL(const std::string&) {
		switch (mWaveType->getSelectedOption()) {
			case 0:
				return "sine_wave(sample0, 440, 1.0f)";
				break;
			case 1:
				return "Tramp_up(sample0, 440, 1.0f)";
				break;
			case 2:
				return "Tramp_down(sample0, 440, 1.0f)";
				break;
			case 3:
				return "Ttriangle(sample0, 440, 1.0f)";
				break;
			case 4:
				return "Tsquare(sample0, 440, 1.0f)";
				break;
			case 5:
				return "white(sample0, 1.0f)";
				break;
			default:
				throw std::runtime_error("Invalid wave type");
		}
	}
}
