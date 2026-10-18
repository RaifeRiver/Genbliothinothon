#include "WaveGenerator.h"

#include <cmath>

#include "part/Background.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	WaveGenerator::WaveGenerator(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 140.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 140.0f}, "Wave generator"));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 46.0f}, glm::vec2{194.0f, 20.0f}, "Audio"));
		mFrequency = &addPart(std::make_unique<part::NoteDial>(this, glm::vec2{30.0f, 74.0f}, glm::vec2{50.0f, 50.0f}, "Frequency"));
		mVolume = &addPart(std::make_unique<part::Dial>(this, glm::vec2{110.0f, 74.0f}, glm::vec2{50.0f, 50.0f}, "Volume", 0.0f, 1.0f, 1.0f));
		mWaveType = &addPart(std::make_unique<part::Dropdown>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, std::vector<std::string>{"Sine", "Saw", "Inv Saw", "Triangle", "Square", "White noise"}));
	}

	std::string WaveGenerator::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		const auto frequencyIt = inputValues.find("Frequency");
		const auto volumeIt = inputValues.find("Volume");
		std::string frequency = mFrequency->getDefaultValueString();
		std::string volume = mVolume->getDefaultValueString();
		if (frequencyIt != inputValues.end()) {
			if (const auto semitoneOffset = static_cast<int32_t>(std::round(mFrequency->getValue())) - 60; semitoneOffset == 0) {
				frequency = frequencyIt->second;
			}
			else {
				// 27.5f * std::pow(2.0f, (std::round(getValue()) - 1.0f) / 12.0f)
				frequency = std::string("27.5f*pow(2.0f,(12.0f*log2(") + frequencyIt->second + "/27.5f)+" + std::to_string(semitoneOffset) + ")/12.0f)";
			}
		}
		if (volumeIt != inputValues.end()) {
			volume += "*(" + volumeIt->second + ")";
		}
		const std::string indexStr = std::to_string(index);
		switch (mWaveType->getSelectedOption()) {
			case 0:
				return "float Audio" + indexStr + "=sine(sample0, " + frequency + ", " + volume + ");";
				break;
			case 1:
				return "float Audio" + indexStr + "=saw(sample0, " + frequency + ", " + volume + ");";
				break;
			case 2:
				return "float Audio" + indexStr + "=Isaw(sample0, " + frequency + ", " + volume + ");";
				break;
			case 3:
				return "float Audio" + indexStr + "=tri(sample0, " + frequency + ", " + volume + ");";
				break;
			case 4:
				return "float Audio" + indexStr + "=sq(sample0, " + frequency + ", " + volume + ");";
				break;
			case 5:
				return "float Audio" + indexStr + "=white(sample0, " + volume + ");";
				break;
			default:
				throw std::runtime_error("Invalid wave type");
		}
	}
}
