#include "AHDSR.h"

#include "Genbliothinothon.h"
#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	AHDSR::AHDSR(const glm::vec2& pos) : Module(pos, {400.0f, 162.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{400.0f, 162.0f}, "AHDSR"));
		addPart(std::make_unique<part::RemoveButton>(this));
		mVolume = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{194.0f, 20.0f}, "Volume", "0.0f", true));
		mNoteStart = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 70.0f}, glm::vec2{194.0f, 20.0f}, "NoteStart", "0", true));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 139.0f}, glm::vec2{194.0f, 20.0f}, "Volume"));
		mNoteHeld = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 93.0f}, glm::vec2{194.0f, 20.0f}, "NoteHeld", "false", true));
		mNoteHeld->setCombiningMode(part::CombiningMode::OR);
		mFrequency = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, "Frequency", "0.0f", true));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 116.0f}, glm::vec2{194.0f, 20.0f}, "Frequency"));
	}

	AHDSR::AHDSR(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		const std::vector<std::unique_ptr<part::Part>>& parts = getParts();
		if (version == 0) {
			setSize({200.0f, 116.0f});
			parts[0]->setSize({200.0f, 116.0f});
			parts[4]->setPos({3.0f, 93.0f});
			mNoteHeld = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 70.0f}, glm::vec2{194.0f, 20.0f}, "NoteHeld", "false", true));
			mNoteHeld->setCombiningMode(part::CombiningMode::OR);
		}
		mVolume = reinterpret_cast<part::GenericSendTarget*>(parts[2].get());
		mNoteStart = reinterpret_cast<part::GenericSendTarget*>(parts[3].get());
	}

	std::string AHDSR::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		std::string glsl = "float Volume0_" + std::to_string(index) + "=";
		uint32_t channelCount = 0;
		for (const std::vector<part::PartID>& volumeSends = mVolume->getInputs(); const auto [module, part]: volumeSends) {
			channelCount = std::max(channelCount, reinterpret_cast<part::Send*>(Genbliothinothon::getInstance()->getModuleManager().getModule(module)->getParts()[part].get())->getChannelCount());
		}
		for (uint32_t channel = 0; channel < channelCount; channel++) {
			std::string channelStr = std::to_string(channel);
			const auto volumeIt = inputValues.find("Volume" + channelStr + "_");
			const auto noteStartIt = inputValues.find("NoteStart" + channelStr + "_");
			const auto noteHeldIt = inputValues.find("NoteHeld" + channelStr + "_");
			const auto frequencyIt = inputValues.find("Frequency" + channelStr + "_");
			std::string volume = "0.0f";
			std::string noteStart = "0";
			std::string noteHeld = "false";
			std::string frequency = "0.0";
			if (volumeIt != inputValues.end()) {
				volume = volumeIt->second;
			}
			if (noteStartIt != inputValues.end()) {
				noteStart = noteStartIt->second;
			}
			if (noteHeldIt != inputValues.end()) {
				noteHeld = noteHeldIt->second;
			}
			if (frequencyIt != inputValues.end()) {
				frequency = frequencyIt->second;
			}
			glsl += "AHDSR(" + volume + ",sample0-" += noteStart + ",50.0f,-10.0f,300.0f,200.0f,10.0f,0.7f,6000.0f,-5.0f," += noteHeld + frequency +")+";
		}
		glsl.back() = ';';
		return glsl;
	}
}
