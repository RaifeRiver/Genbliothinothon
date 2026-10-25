#include "Oscillator.h"

#include <cmath>

#include "Genbliothinothon.h"
#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	Oscillator::Oscillator(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 140.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 140.0f}, "Oscillator"));
		addPart(std::make_unique<part::RemoveButton>(this));
		mAudio = &addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 46.0f}, glm::vec2{194.0f, 20.0f}, "Amplitude", 16));
		mFrequency = &addPart(std::make_unique<part::NoteDial>(this, glm::vec2{30.0f, 74.0f}, glm::vec2{50.0f, 50.0f}, "Tone", true));
		mVolume = &addPart(std::make_unique<part::Dial>(this, glm::vec2{110.0f, 74.0f}, glm::vec2{50.0f, 50.0f}, "Volume", 0.0f, 1.0f, 1.0f, true));
		mWaveType = &addPart(std::make_unique<part::Dropdown>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, std::vector<std::string>{"Sine", "Saw", "Inv Saw", "Triangle", "Square"}));
	}

	Oscillator::Oscillator(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mAudio = reinterpret_cast<part::Send*>(getParts()[2].get());
		mFrequency = reinterpret_cast<part::NoteDial*>(getParts()[3].get());
		mVolume = reinterpret_cast<part::Dial*>(getParts()[4].get());
		mWaveType = reinterpret_cast<part::Dropdown*>(getParts()[5].get());
	}

	std::string Oscillator::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		std::string functionName;
		switch (mWaveType->getSelectedOption()) {
			case 0:
				functionName = "sine";
				break;
			case 1:
				functionName = "saw";
				break;
			case 2:
				functionName = "Isaw";
				break;
			case 3:
				functionName = "tri";
				break;
			case 4:
				functionName = "sq";
				break;
			default:
				throw std::runtime_error("Invalid wave type");
		}
		const std::string defaultFrequency = mFrequency->getDefaultValueString();
		const std::string defaultVolume = mVolume->getDefaultValueString();
		const std::string indexStr = std::to_string(index);
		std::string glsl;
		for (uint32_t channel = 0; channel < 16; channel++) {
			std::string channelStr = std::to_string(channel);
			const auto toneIt = inputValues.find("Tone" + channelStr + "_");
			const auto volumeIt = inputValues.find("Volume" + channelStr + "_");
			std::string frequency = defaultFrequency;
			std::string volume = defaultVolume;
			if (toneIt != inputValues.end()) {
				if (const auto semitoneOffset = static_cast<int32_t>(std::round(mFrequency->getValue())) - 60; semitoneOffset == 0) {
					frequency = toneIt->second;
				}
				else {
					frequency = std::string("27.5f*pow(2.0f,(12.0f*log2(") + toneIt->second + "/27.5f)+" + std::to_string(semitoneOffset) + ")/12.0f)";
				}
			}
			if (volumeIt != inputValues.end()) {
				volume += "*(" + volumeIt->second + ")";
			}
			glsl += "float Amplitude" + channelStr + "_" += indexStr + "=" += functionName + "(sample0," += frequency + "," += volume + ");\n";
		}
		return glsl;
	}
}
