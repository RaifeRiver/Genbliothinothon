#include "Tremlo.h"

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	Tremlo::Tremlo(const glm::vec2& pos) : Module(pos, glm::vec2{265.0f, 118.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{265.0f, 118.0f}, "Tremlo"));
		addPart(std::make_unique<part::RemoveButton>(this));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{259.0f, 20.0f}, "Audio"));
		mAmplitude = &addPart(std::make_unique<part::Dial>(this, glm::vec2{10.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Amplitude", 0.0f, 1.0f, 1.0f, true));
		mPeriod = &addPart(std::make_unique<part::Dial>(this, glm::vec2{75.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Period", 0.1f, 10.0f, 1.0f, true));
		mGain = &addPart(std::make_unique<part::Dial>(this, glm::vec2{140.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Gain", 0.1f, 10.0f, 1.0f, true));
		mSmooth = &addPart(std::make_unique<part::Dial>(this, glm::vec2{205.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Smooth", 0.1f, 10.0f, 1.0f, true));
	}

	Tremlo::Tremlo(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mAmplitude = reinterpret_cast<part::Dial*>(getParts()[3].get());
		mPeriod = reinterpret_cast<part::Dial*>(getParts()[4].get());
		mGain = reinterpret_cast<part::Dial*>(getParts()[5].get());
		mSmooth = reinterpret_cast<part::Dial*>(getParts()[6].get());
	}

	std::string Tremlo::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		std::string glsl;
		for (uint32_t channel = 0; channel < 16; channel++) {
			std::string channelStr = std::to_string(channel);
			const auto amplitudeIt = inputValues.find("Amplitude"+channelStr+"_");
			const auto periodIt = inputValues.find("Period"+channelStr+"_");
			const auto gainIt = inputValues.find("Gain"+channelStr+"_");
			const auto smoothIt = inputValues.find("Smooth"+channelStr+"_");
			std::string amplitude = mAmplitude->getDefaultValueString();
			std::string period = mPeriod->getDefaultValueString();
			std::string gain = mGain->getDefaultValueString();
			std::string smooth = mSmooth->getDefaultValueString();
			if (amplitudeIt != inputValues.end()) {
				amplitude += "*(" + amplitudeIt->second + ")";
			}
			if (periodIt != inputValues.end()) {
				period += "*(" + periodIt->second + ")";
			}
			if (gainIt != inputValues.end()) {
				gain += "*(" + gainIt->second + ")";
			}
			if (smoothIt != inputValues.end()) {
				smooth += "*(" + smoothIt->second + ")";
			}
			const std::string indexStr = std::to_string(index);
			glsl += "vec2 Data" + channelStr + "_" + indexStr + " = Tremlo(SampleI, " + amplitude + ", " + period + ", " + gain + ", " + smooth + ");\n" + "float Amplitude" + channelStr + "_" + indexStr + " = Data" + channelStr + "_" + indexStr + ".x;\n" + "float Frequency" + channelStr + "_" + indexStr + " = Data" + channelStr + "_" + indexStr + ".y;\n";
		}
		return glsl;
	}
}
