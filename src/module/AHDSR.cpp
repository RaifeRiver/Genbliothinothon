#include "AHDSR.h"

#include "Genbliothinothon.h"
#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	AHDSR::AHDSR(const glm::vec2& pos) : Module(pos, {299.0f, 195.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{299.0f, 195.0f}, "AHDSR"));
		addPart(std::make_unique<part::RemoveButton>(this));
		// colum 1 row 2
		mVolume = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{145.0f, 20.0f}, "Volume", "0.0f", true));
		// colum 2 row 1
		mNoteStart = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{151.0f, 24.0f}, glm::vec2{145.0f, 20.0f}, "NoteStart", "0", true));
		// colum 2 row 3
		addPart(std::make_unique<part::Send>(this, glm::vec2{151.0f, 172.0f}, glm::vec2{145.0f, 20.0f}, "Volume"));
		// colum 2 row 2
		mNoteHeld = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{151.0f, 47.0f}, glm::vec2{145.0f, 20.0f}, "NoteHeld", "false", true));
		mNoteHeld->setCombiningMode(part::CombiningMode::OR);
		// colum 1 row 1
		mFrequency = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{145.0f, 20.0f}, "Frequency", "0.0f", true));
		// colum 1 row 3
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 172.0f}, glm::vec2{145.0f, 20.0f}, "Frequency"));

		mAT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{3.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "AT", 0.0f, 20000.0f, 50.0f, true));
		mAC = &addPart(std::make_unique<part::Dial>(this, glm::vec2{38.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "AC", -50.0f, 50.0f, -10.0f, true));
		mHT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{73.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "HT", 0.0f, 20000.0f, 1.0f, true));
		mSP = &addPart(std::make_unique<part::Dial>(this, glm::vec2{108.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "SP", 0.0f, 1.0f, 0.7f, true));
		mDT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{143.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "DT", 0.0f, 20000.0f, 500.0f, true));
		mDC = &addPart(std::make_unique<part::Dial>(this, glm::vec2{178.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "DC", -50.0f, 50.0f, 4.0f, true));
		mRT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{213.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "RT", 0.0f, 20000.0f, 600.0f, true));
		mRC = &addPart(std::make_unique<part::Dial>(this, glm::vec2{248.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "RC", -50.0f, 50.0f, -10.0f, true));

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
			std::string AT = mAT->getDefaultValueString();
			std::string AC = mAC->getDefaultValueString();
			std::string HT = mHT->getDefaultValueString();
			std::string SP = mSP->getDefaultValueString();
			std::string DT = mDT->getDefaultValueString();
			std::string DC = mDC->getDefaultValueString();
			std::string RT = mRT->getDefaultValueString();
			std::string RC = mRC->getDefaultValueString();
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
			//float AT, float AC, float HT, float DT, float DC, float SP, float RT, float RC
			glsl += "AHDSR(" + volume + ",sample0-" += noteStart + "AT" + "AC" + "HT" + "DT" + "DC" + "SP" + "RT" + "RC" += noteHeld + frequency +")+";
		}
		glsl.back() = ';';
		return glsl;
	}
}
