#include "WaveGenerator.h"

#include "part/Background.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	WaveGenerator::WaveGenerator(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 150.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 150.0f}, "Wave generator"));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 46.0f}, glm::vec2{194.0f, 20.0f}));
		mFrequency = &addPart(std::make_unique<part::Dial>(this, glm::vec2{3.0f, 66.0f}, glm::vec2{50.0f, 50.0f}, "Frequency", 1.0f, 20000.0f, 440.0f));
		mWaveType = &addPart(std::make_unique<part::Dropdown>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, std::vector<std::string>{"Sine", "Saw", "Inv Saw", "Triangle", "Square", "White noise"}));
	}

	std::string WaveGenerator::generateGLSL(std::unordered_map<std::string, std::string>& inputValues) {
		const std::string frequency = inputValues["Frequency"];
		switch (mWaveType->getSelectedOption()) {
			case 0:
				return "sine(sample0, " + frequency + ", 1.0f)";
				break;
			case 1:
				return "saw(sample0, " + frequency + ", 1.0f)";
				break;
			case 2:
				return "Isaw(sample0, " + frequency + ", 1.0f)";
				break;
			case 3:
				return "tri(sample0, " + frequency + ", 1.0f)";
				break;
			case 4:
				return "sq(sample0, " + frequency + ", 1.0f)";
				break;
			case 5:
				return "white(sample0, 1.0f)";
				break;
			default:
				throw std::runtime_error("Invalid wave type");
		}
	}
}
