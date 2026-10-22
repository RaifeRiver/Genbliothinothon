#include "AHDSR.h"

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	AHDSR::AHDSR(const glm::vec2& pos) : Module(pos, {200.0f, 93.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 93.0f}, "AHDSR"));
		addPart(std::make_unique<part::RemoveButton>(this));
		mAmplitude = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, "Amplitude", "0.0f", true));
		mNoteStart = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{194.0f, 20.0f}, "NoteStart", "0", true));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 70.0f}, glm::vec2{194.0f, 20.0f}, "Amplitude"));
	}

	AHDSR::AHDSR(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mAmplitude = reinterpret_cast<part::GenericSendTarget*>(getParts()[2].get());
		mNoteStart = reinterpret_cast<part::GenericSendTarget*>(getParts()[3].get());
	}

	std::string AHDSR::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		std::string glsl = "float Amplitude0_" + std::to_string(index) + "=";
		uint32_t channelCount = 0;
		for (const std::vector<part::Send*>& amplitudeSends = mAmplitude->getInputs(); const part::Send* amplitudeSend: amplitudeSends) {
			channelCount = std::max(channelCount, amplitudeSend->getChannelCount());
		}
		for (uint32_t channel = 0; channel < channelCount; channel++) {
			std::string channelStr = std::to_string(channel);
			const auto amplitudeIt = inputValues.find("Amplitude" + channelStr + "_");
			const auto noteStartIt = inputValues.find("NoteStart" + channelStr + "_");
			std::string amplitude = "0.0f";
			std::string noteStart = "0";
			if (amplitudeIt != inputValues.end()) {
				amplitude = amplitudeIt->second;
			}
			if (noteStartIt != inputValues.end()) {
				noteStart = noteStartIt->second;
			}
			glsl += "AHDSR(" + amplitude + ",sample0-" += noteStart + ",50.0f,-10.0f,300.0f,200.0f,10.0f,0.7f,6000.0f,-5.0f,false)+";
		}
		glsl.back() = ';';
		return glsl;
	}
}
