#include "NoiseSource.h"

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	NoiseSource::NoiseSource(const glm::vec2& pos) : Module(pos, glm::vec2{200.0f, 118.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 118.0f}, "Noise Source"));
		addPart(std::make_unique<part::RemoveButton>(this));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, "Audio"));
		mVolume = &addPart(std::make_unique<part::Dial>(this, glm::vec2{10.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Volume", 0.0f, 1.0f, 1.0f));
		mLowPass = &addPart(std::make_unique<part::Dial>(this, glm::vec2{75.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "Low Pass", 10.0f, 20000.0f, 20000.0f));
		mHighPass = &addPart(std::make_unique<part::Dial>(this, glm::vec2{140.0f, 52.0f}, glm::vec2{50.0f, 50.0f}, "High Pass", 10.0f, 20000.0f, 10.0f));
	}

	std::string NoiseSource::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		const auto volumeIt = inputValues.find("Volume");
		const auto lowPassIt = inputValues.find("Low Pass");
		const auto highPassIt = inputValues.find("High Pass");
		std::string volume = mVolume->getDefaultValueString();
		std::string lowPass = mLowPass->getDefaultValueString();
		std::string highPass = mHighPass->getDefaultValueString();
		if (volumeIt != inputValues.end()) {
			volume += "*(" + volumeIt->second + ")";
		}
		if (lowPassIt != inputValues.end()) {
			lowPass += "*(" + lowPassIt->second + ")";
		}
		if (highPassIt != inputValues.end()) {
			highPass += "*(" + highPassIt->second + ")";
		}
		const std::string indexStr = std::to_string(index);
		return "float Audio" + indexStr + "=0.0f;\nVOICE_BEGIN(white(sampleI," + volume + "))\nVOICE_FILTER(FILTER_LOWPASS," + lowPass + ",0.0f,1.0f)\nVOICE_FILTER(FILTER_HIGHPASS," + highPass + ",0.0f,1.0f)\nVOICE_END(Audio" + indexStr + ")";
	}
}
