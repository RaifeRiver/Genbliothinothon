#include "FullRectifier.h"

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	FullRectifier::FullRectifier(const glm::vec2& pos) : Module(pos, glm::vec2{200.0f, 118.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 118.0f}, "FullRectifier"));
		addPart(std::make_unique<part::RemoveButton>(this));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, "Audio",16));
		mAmplitude = &addPart(std::make_unique<part::Dial>(this, glm::vec2{10.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Amplitude", 0.0f, 1.0f, 1.0f, true));
		mDrive = &addPart(std::make_unique<part::Dial>(this, glm::vec2{75.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Drive", 0.1f, 10.0f, 1.0f, true));
		mGain = &addPart(std::make_unique<part::Dial>(this, glm::vec2{140.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Gain", 0.1f, 10.0f, 1.0f, true));
	}

	FullRectifier::FullRectifier(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mAmplitude = reinterpret_cast<part::Dial*>(getParts()[3].get());
		mDrive = reinterpret_cast<part::Dial*>(getParts()[4].get());
		mGain = reinterpret_cast<part::Dial*>(getParts()[5].get());
	}

	std::string FullRectifier::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		std::string glsl;
		for (uint32_t channel = 0; channel < 16; channel++) {
			std::string channelStr = std::to_string(channel);
			const auto volumeIt = inputValues.find("Amplitude"+channelStr+"_");
			const auto driveIt = inputValues.find("Drive"+channelStr+"_");
			const auto gainIt = inputValues.find("Gain"+channelStr+"_");
			std::string volume = mAmplitude->getDefaultValueString();
			std::string drive = mDrive->getDefaultValueString();
			std::string gain = mGain->getDefaultValueString();
			if (volumeIt != inputValues.end()) {
				volume += "*(" + volumeIt->second + ")";
			}
			if (driveIt != inputValues.end()) {
				drive += "*(" + driveIt->second + ")";
			}
			if (gainIt != inputValues.end()) {
				gain += "*(" + gainIt->second + ")";
			}
			const std::string indexStr = std::to_string(index);
			glsl+= "float Audio" + channelStr + "_" += indexStr + "=Antidepressant(" += volume + "," += drive + "," += gain + ");\n";
		}
		return glsl;
	}
}
